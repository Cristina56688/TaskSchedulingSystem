#include "header.h"

#define WTFILE "waitingtasklist.xml"



void load_tasks_from_file(std::vector<Task>& tasks) {
    std::cout << "[PLANIFICATOR] Reincarc task-urile din fisier...\n";
    tasks=extract_waiting_tasks();
}


void* run_scheduler(void* arg)
{
    std::cout<<"[PLANIFICATOR]: start\n";

    std::vector<Task> tasks=extract_waiting_tasks();
    time_t last_time = 0;

    while (true) {
        if (reload_needed.load()) {
            reload_needed.store(false);
            load_tasks_from_file(tasks);
        }

        time_t now = time(0);
        if (now != last_time) {
            last_time = now;
            tm *ltm = localtime(&now);

            std::vector<ExecTask> batch_tasks;
            std::vector<int> tasks_to_remove;

            for (auto it = tasks.begin(); it != tasks.end(); ) {
                const auto& t = *it;
                
                struct tm task_tm = *ltm; 
                task_tm.tm_year = t.getData().getAn() - 1900;
                task_tm.tm_mon = t.getData().getLuna() - 1;
                task_tm.tm_mday = t.getData().getZi();
                task_tm.tm_hour = t.getOra().getOra();
                task_tm.tm_min = t.getOra().getMin();
                task_tm.tm_sec = t.getOra().getSec();
                
                time_t task_time = mktime(&task_tm);

                if (task_time <= now) {
                    if (now - task_time > 3600) {
                        std::cout << "[PLANIFICATOR] Task " << t.getId() << " is too old (" << (now - task_time) << "s ago). Discarding.\n";
                        remove_task(t.getId());
                        it = tasks.erase(it);
                        continue;
                    }
                    
                    std::stringstream ss;
                    for(const auto& s : t.getTask()) ss << s << " ";
                    
                    ExecTask execTask;
                    execTask.id = t.getId();
                    execTask.command = ss.str();
                    execTask.user = t.getUserName();
                    execTask.priority = t.getPriority(); 
                    execTask.timestamp = (long long)time(nullptr);

                    batch_tasks.push_back(execTask);
                    
                    std::cout << "[PLANIFICATOR] Sent task " << t.getId() << " to executor (Priority: " << execTask.priority << ", User: " << execTask.user << ").\n";
                    
                    tasks_to_remove.push_back(t.getId());
                    it = tasks.erase(it);
                } else {
                    ++it;
                }
            }

           
            if (!batch_tasks.empty()) {
                {
                    LockGuard lock(queueMutex);
                    for (const auto& et : batch_tasks) {
                        if (std::find(userList.begin(), userList.end(), et.user) == userList.end()) {
                            userList.push_back(et.user);
                        }
                        userQueues[et.user].push(et);
                    }
                }
                queueCond.notify_all();
         
                for (int id : tasks_to_remove) {
                    remove_task(id);
                }
            }
        }

        usleep(100000); 
    }
    return nullptr;
}