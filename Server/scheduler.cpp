#include "header.h"

#define WTFILE "waitingtasklist.xml"

std::atomic<bool> reload_needed(false);

std::vector<Task> tasks;

void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        reload_needed.store(true);
    }
}

void save_pid() {

    int fd=open("planificator.pid", O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fd<0)
    {
        std::cerr<<"Eroare in deschiderea fisierului planificator.pid";
        exit(1);
    }

    std::string pid=std::to_string(getpid());
    write(fd, pid.c_str(), pid.size());

    close(fd);
}

void load_tasks_from_file() {
    std::cout << "[PLANIFICATOR] Reincarc task-urile din fisier...\n";
    tasks=extract_waiting_tasks();
}


int main()
{
    std::cout<<"[PLANIFICATOR]: strat\n";
    save_pid();

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, nullptr);

    std::vector<Task> tasks=extract_waiting_tasks();

     
    while (true) {
        if (reload_needed.load()) {
            reload_needed.store(false);
            load_tasks_from_file();
        }

        // logica de planificare

        usleep(100000); 
    }

    return 0;
}