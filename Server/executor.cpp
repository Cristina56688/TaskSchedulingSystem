#include "header.h"
#include <sys/stat.h>
#include <sys/types.h>

#define HISTORY_FILE "history.xml"

Mutex xmlMutex;

void log_history(int id, const std::string& cmd, int exit_code) {
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
    
    // Add timestamp
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
        {
            LockGuard lock(queueMutex);
            queueCond.wait(queueMutex, []{ return !taskQueue.empty(); });
            t = taskQueue.front();
            taskQueue.pop();
        }

        std::cout << "[EXECUTOR] Executing task " << t.id << ": " << t.command << "\n";

        pid_t pid = fork();
        if (pid == 0) {
            // Copil
            std::vector<std::string> parts;
            
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
            // Parent
            int status;
            waitpid(pid, &status, 0);
            int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
            std::cout << "[EXECUTOR] Task " << t.id << " finished with code " << exit_code << "\n";
            log_history(t.id, t.command, exit_code);
        } else {
            perror("fork");
        }
    }
    return nullptr;
}

void* run_executor(void* arg) {
    std::cout << "[EXECUTOR] Starting...\n";
    
    // 4 thread uri
    const int NUM_THREADS = 4;
    std::vector<pthread_t> threads(NUM_THREADS);
    for(int i=0; i<NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], nullptr, worker_routine, nullptr) != 0) {
            perror("pthread_create worker");
        }
    }

    // Join thread
    for(auto& t : threads) {
        pthread_join(t, nullptr);
    }
    return nullptr;
}
