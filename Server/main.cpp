#include "header.h"
#include <iostream>

std::priority_queue<ExecTask> taskQueue;
Mutex queueMutex;
ConditionVariable queueCond;
std::atomic<bool> reload_needed(false);

int main() {
    std::cout << "[MAIN] Starting Task Scheduling System...\n";

  
    pthread_t schedulerThread;
    if (pthread_create(&schedulerThread, nullptr, run_scheduler, nullptr) != 0) {
        perror("pthread_create scheduler");
        return 1;
    }


    pthread_t executorThread;
    if (pthread_create(&executorThread, nullptr, run_executor, nullptr) != 0) {
        perror("pthread_create executor");
        return 1;
    }

  
    server_main();

    pthread_join(schedulerThread, nullptr);
    pthread_join(executorThread, nullptr);

    return 0;
}
