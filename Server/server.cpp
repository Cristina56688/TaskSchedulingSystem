#include "header.h"

#define ACCF "accountslist.xml"
#define WTFILEILE "waitingtasklist.xml"
#define PORT 54000


static void reap_children(int) { //reaping
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        
    }
}

static bool run_task_and_stream_to_client(sf::TcpSocket& client, char* const argv[]) {
    int pfd[2];
    if (pipe(pfd) == -1) {
        std::string err = "TASK_ERR|pipe_failed\n";
        send_message(client, err);
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(pfd[0]);
        close(pfd[1]);
        std::string err = "TASK_ERR|fork_failed\n";
        send_message(client, "5");
        return false;
    }

    if (pid == 0) {
        // copil-task
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        dup2(pfd[1], STDERR_FILENO);


        //***********************
        //************FUNCTIE DE EXECUTIE CARE SA ACCEPTE PIPEURI*************
        //******************* */

        perror("execvp");
        _exit(0);
    }


    close(pfd[1]);
    // citim continuu din pipe si retransmitem clientului
    char buf[4096];   //dimensiune tipica a unei pagini de memorie pt optimizare
    for (;;) {
        ssize_t n = read(pfd[0], buf, sizeof(buf));
        if (n > 0) {

            // ***************
            // Trimitere output la clent
            // salvare output
            //******************************* */

        } else if (n == 0) {
            break; // EOF – copilul a terminat
        } else {
            if (errno == EINTR) continue;  //syscall intrerupere
            break;
        }
    }
    close(pfd[0]);

    // colectăm copilul-task (blocking OK aici — tocmai s-a închis pipe-ul)
    int status = 0;
    waitpid(pid, &status, 0);

    //************* */
    //salvare stare de terminare
    //***************** */

    return true;
}

bool notify_scheduler(const char* pidfile = "planificator.pid") {


    int fd=open("planificator.pid", O_RDONLY);
    if(fd<0)
    {
        std::cerr<<"eroare in deschiderea fisierului planificator.pid";
        exit(1);
    }

    char* pid_str=(char*)malloc(10);
    int bytes=read(fd, pid_str, 10);
    pid_str[bytes]='\0';


    if (kill(stoi(std::string(pid_str)), SIGUSR1) != 0) {
        perror("[SERVER] kill esuat");
        return false;
    }

    std::cout << "[SERVER] Am trimis SIGUSR1 la planificator (pid=" << pid_str << ")\n";

    free(pid_str);
    return true;
}


static bool handle_request(sf::TcpSocket& client, const std::string& payload) {
    try {
        int index = getIndex(payload);
        std::string userName;

        switch (index) {
        case 0: { // LOGIN  0||user||pass
            LogInData logIn(payload);
            if (verifyAccount(ACCF, logIn)) {
                std::string message=create_message_login(logIn.getUserName());
                send_message(client, message); // 2||nr tasks||task||data||ora||...
            } else {
                send_message(client, "3"); // eroare login
            }
            break;
        }
        case 1: { // SIGNIN 1||user||mail||pass
            SingInData singIn(payload);
            if (addAccount(ACCF, singIn)) {
                send_message(client, "0"); // succes signin
                sendAccountCreationConfirmation(singIn.getMail());
            } else {
                send_message(client, "1"); // eroare signin
            }
            break;
        }
        case 2: 
        {
            std::cout<<"Client deconectat.\n";
            return false;
        }
        case 3: {
            try{
                Task task(payload);
                save_task(WTFILEILE, task);
                notify_scheduler();
            }
            catch(const std::exception& e)
            {
                std::cerr<<e.what()<<std::endl;
            }

            std::cout<<"Task primit: "<<payload<<std::endl;
            break;
        }
        default:
            break;
        }
    } catch (const std::exception& e) {
        std::cerr << "[E] handle_request: " << e.what() << "\n";
        // +inchidere seiune erori grave (return false)
    }
    return true; 
}

// copil sesiune 
static void session_loop(sf::TcpListener& listener_to_close, sf::TcpSocket& client) {
   
    listener_to_close.close();  //copilul nu trebuie sa accepte sesiuni noi

    // Reap pentru copiii-task ai acestui copil-sesiune
    struct sigaction sa{};
    sa.sa_handler = reap_children;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, nullptr);

    for (;;) {
        std::string msg;
        if (!recv_message(client, msg)) {
            // client închis sau eroare
            break;
        }
        if (!handle_request(client, msg)) {
            break;
        }
    }

    client.disconnect();
    _exit(0);
}



int main() {

    struct sigaction sa{};
    sa.sa_handler = reap_children;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, nullptr);

    pid_t pid_scheduler=fork();
    if(pid_scheduler==0)
    {
        execl("./scheduler", "./scheduler", NULL);
        perror("exec failed");
    }

    sf::TcpListener listener;
    if (listener.listen(PORT) != sf::Socket::Done) {
        std::cerr << "Eroare la bind!\n";
        return 1;
    }
    std::cout << "Server pornit pe portul " << PORT << "...\n";

    for (;;) {


        sf::TcpSocket client;
        auto st = listener.accept(client);
        if (st != sf::Socket::Done) continue;

        // Pentru fiecare client → copil-sesiune
        pid_t pid = fork();
        if (pid < 0) {
            std::perror("fork");
            client.disconnect();
            continue;
        }
        if (pid == 0) {
            std::cout << "[child] sesiune noua PID=" << getpid() << "\n";
            session_loop(listener, client);
        } else {
            client.disconnect();
        }
    }

    wait(nullptr);

    return 0;
}
