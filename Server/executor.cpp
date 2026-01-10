#include "header.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>

#define HISTORY_FILE "history.xml"

Mutex xmlMutex;

void log_history(int id, const std::string& cmd, int exit_code, const std::string& user, long long duration_ms) {
    LockGuard lock(xmlMutex);
    XMLDocument doc;
    XMLError err = doc.LoadFile(HISTORY_FILE);
    XMLElement* root;
    if (err != XML_SUCCESS) {
        root = doc.NewElement("History");
        doc.InsertFirstChild(root);
    } else {
        root = doc.FirstChildElement("History");
    }

    XMLElement* entry = doc.NewElement("Entry");
    entry->SetAttribute("ID", id);
    entry->SetAttribute("Command", cmd.c_str());
    entry->SetAttribute("ExitCode", exit_code);
    entry->SetAttribute("User", user.c_str());
    entry->SetAttribute("DurationMs", (int64_t)duration_ms);
    
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    entry->SetAttribute("Time", buf);

    root->InsertEndChild(entry);
    doc.SaveFile(HISTORY_FILE);
}

void* worker_routine(void* arg) {
    while (true) {
        ExecTask t;
        bool found = false;
        {
            LockGuard lock(queueMutex);
            queueCond.wait(queueMutex, []{
                for (const auto& user : userList) {
                    if (!userQueues[user].empty()) return true;
                }
                return false;
            });

            int n = userList.size();
            for (int i = 0; i < n; ++i) {
                int idx = (currentUserIndex + i) % n;
                const std::string& user = userList[idx];
                if (!userQueues[user].empty()) {
                    t = userQueues[user].front();
                    userQueues[user].pop_front();
                    currentUserIndex = (idx + 1) % n;
                    found = true;
                    break;
                }
            }
        }
        if (!found) continue;

        std::cout << "[EXECUTOR] Handling task " << t.id << " (User: " << t.user << ", Suspended: " << (t.suspended ? "Yes" : "No") << ")\n";

        if (!t.suspended) {
            pid_t pid = fork();
            if (pid == 0) {
                setpgid(0, 0); 
                const char* shell = "/bin/sh";
                std::vector<char*> args;
                args.push_back(const_cast<char*>(shell));
                args.push_back(const_cast<char*>("-c"));
                args.push_back(const_cast<char*>(t.command.c_str()));
                args.push_back(nullptr);
                execvp(shell, args.data());
                perror("execvp");
                _exit(1);
            } else if (pid > 0) {
                t.pid = pid;
            } else {
                perror("fork");
                continue;
            }
        } else {
            
            if (kill(-t.pid, 0) == -1 && errno == ESRCH) {
                std::cout << "[EXECUTOR] Task " << t.id << " group already dead. Cleaning up.\n";
                log_history(t.id, t.command, 0, t.user, t.total_duration_ms); 
                continue;
            }
            std::cout << "[EXECUTOR] Resuming task " << t.id << " (PGID: " << t.pid << ")\n";
            kill(-t.pid, SIGCONT);
        }

        auto start_time = std::chrono::steady_clock::now();
        int status = 0;
        bool leader_finished = false;
        bool group_finished = false;
        bool preempted = false;

        while (true) {
            if (!leader_finished) {
                pid_t res = waitpid(t.pid, &status, WNOHANG);
                if (res == t.pid) {
                    leader_finished = true;
                } else if (res == -1) {
                    if (errno == ECHILD) {
                        leader_finished = true; 
                    } else if (errno != EINTR) {
                        perror("waitpid");
                        leader_finished = true;
                    }
                }
            }
            
            if (kill(-t.pid, 0) == -1 && errno == ESRCH) {
                group_finished = true;
            }

            if (leader_finished && group_finished) {
                break;
            }

            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
            if (elapsed >= TIME_QUANTUM_MS) {
                preempted = true;
                break;
            }
            usleep(50000); 
        }

        if (leader_finished && group_finished) {
            int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : 0;
            auto now_end = std::chrono::steady_clock::now();
            t.total_duration_ms += std::chrono::duration_cast<std::chrono::milliseconds>(now_end - start_time).count();

            std::cout << "[EXECUTOR] Task " << t.id << " truly finished (Code: " << exit_code << ", Duration: " << t.total_duration_ms << "ms)\n";
            log_history(t.id, t.command, exit_code, t.user, t.total_duration_ms);
            
            if (t.total_duration_ms > 60000) {
                std::string userMail = getMailByUserName(t.user);
                if (!userMail.empty()) {
                    std::cout << "[EXECUTOR] Task " << t.id << " had abnormal duration (" << t.total_duration_ms << "ms). Sending alert to " << userMail << "\n";
                    sendAbnormalDurationNotification(userMail, t.id, t.total_duration_ms, t.command);
                }
            }

            if (exit_code != 0) {
                std::string userMail = getMailByUserName(t.user);
                if (!userMail.empty()) {
                    std::cout << "[EXECUTOR] Task " << t.id << " failed. Sending mail to " << userMail << "\n";
                    sendTaskFailureNotification(userMail, t.id, exit_code, t.command);
                }
            }
        } else if (preempted) {
            
            if (kill(-t.pid, 0) == 0) {
                std::cout << "[EXECUTOR] Time quantum expired for task " << t.id << ". Suspending group...\n";
                kill(-t.pid, SIGSTOP);
                t.suspended = true;

                auto now_preempt = std::chrono::steady_clock::now();
                t.total_duration_ms += std::chrono::duration_cast<std::chrono::milliseconds>(now_preempt - start_time).count();

                {
                    LockGuard lock(queueMutex);
                    userQueues[t.user].push_back(t);
                }
                queueCond.notify_all();
            } else {
                std::cout << "[EXECUTOR] Task " << t.id << " finished just at quantum expiration.\n";
                auto now_end = std::chrono::steady_clock::now();
                t.total_duration_ms += std::chrono::duration_cast<std::chrono::milliseconds>(now_end - start_time).count();
                log_history(t.id, t.command, 0, t.user, t.total_duration_ms);

                if (t.total_duration_ms > 60000) {
                    std::string userMail = getMailByUserName(t.user);
                    if (!userMail.empty()) {
                        sendAbnormalDurationNotification(userMail, t.id, t.total_duration_ms, t.command);
                    }
                }
            }
        }
    }
    return nullptr;
}

void* run_executor(void* arg) {
    std::cout << "[EXECUTOR] Starting...\n";
    
    const int NUM_THREADS = 8;
    std::vector<pthread_t> threads(NUM_THREADS);
    for(int i=0; i<NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], nullptr, worker_routine, nullptr) != 0) {
            perror("pthread_create worker");
        }
    }

   
    for(auto& t : threads) {
        pthread_join(t, nullptr);
    }
    return nullptr;
}
