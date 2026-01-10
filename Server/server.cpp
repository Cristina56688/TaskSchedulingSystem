#include "header.h"

#define ACCF "accountslist.xml"
#define WTFILEILE "waitingtasklist.xml"
#define PORT 54000

bool notify_scheduler() 
{

     reload_needed.store(true);
    std::cout << "[SERVER] Notificare scheduler (reload_needed=true)\n";
    return true;
}


static bool handle_request(sf::TcpSocket& client, const std::string& payload) {
    try {
        int index = getIndex(payload);
        std::string userName;

        switch (index) {
        case 0: { // login:  0||user||pass
            LogInData logIn(payload);
            if (verifyAccount(ACCF, logIn)) {
                std::string message=create_message_login(logIn.getUserName());
                send_message(client, message); // 2||nr tasks||task||data||ora||...
            } else {
                send_message(client, "3"); // eroare login
            }
            break;
        }
        case 1: { // signin 1||user||mail||pass
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
                send_message(client, "0");
            }
            catch(const std::exception& e)
            {
                std::cerr<<e.what()<<std::endl;
                send_message(client, "1");
            }

            std::cout<<"Task primit: "<<payload<<std::endl;
            break;
        }
        case 4: { // history: 4||username
            std::stringstream ss(payload);
            std::string buff, user;
            getline(ss, buff, '|'); 
            getline(ss, buff, '|'); 
            getline(ss, user, '|'); 
            
            std::string msg = create_message_history(user);
            send_message(client, msg);
            break;
        }
           case 5: { // all tasks: 5||username
            std::stringstream ss(payload);
            std::string buff, user;
            getline(ss, buff, '|'); 
            getline(ss, buff, '|'); 
            getline(ss, user, '|'); 

            std::string msg = create_message_all(user);
            send_message(client, msg);
            break;
        }
        case 6: { // waiting: 6||username
            std::stringstream ss(payload);
            std::string buff, user;
            getline(ss, buff, '|'); 
            getline(ss, buff, '|'); 
            getline(ss, user, '|'); 

            std::string msg = create_message_waiting(user);
            std::cout << "[SERVER] Trimit waiting tasks: " << msg << std::endl;
            send_message(client, msg);
            break;
        }
        case 7: { // delete: 7||id
            std::stringstream ss(payload);
            std::string buff;
             getline(ss, buff, '|'); 
             getline(ss, buff, '|'); 
             getline(ss, buff, '|'); 
             
             try {
                 int id = std::stoi(buff);
                 if(remove_task(id))
                 {
                     send_message(client, "0");
                     notify_scheduler();
                 }
                 else{
                     send_message(client, "1");
                 }
             } catch (...) {
                 std::cerr << "Error parsing delete ID: " << buff << "\n";
                 send_message(client, "1");
             }
             break;
        }
        case 8: { // modify: 8||id||task||data||ora||user||priority
            std::stringstream ss(payload);
            std::string buff, temp_str;
            getline(ss, buff, '|'); 
            getline(ss, buff, '|'); 
            getline(ss, buff, '|'); 
            
            int id = -1;
            try {
                id = std::stoi(buff);
            } catch (...) {
                std::cerr << "Error parsing modify ID: " << buff << "\n";
                send_message(client, "1");
                break;
            }
            
            std::string task_content, data_str, ora_str, user_str, prio_str;
             getline(ss, buff, '|'); 
             getline(ss, task_content, '|');
             getline(ss, buff, '|'); 
             getline(ss, data_str, '|');
             getline(ss, buff, '|'); 
             getline(ss, ora_str, '|');
             getline(ss, buff, '|'); 
             getline(ss, user_str, '|');
             getline(ss, buff, '|'); 
             getline(ss, prio_str, '|');
             
             int priority = 1;
             try { priority = std::stoi(prio_str); } catch(...) {}

           
            std::stringstream ssData(data_str);
            std::string d_aux;
            int zi = 0, luna = 0, an = 0;
            try {
                if(getline(ssData, d_aux, '.')) zi = stoi(d_aux);
                if(getline(ssData, d_aux, '.')) luna = stoi(d_aux);
                if(getline(ssData, d_aux, '.')) an = stoi(d_aux);
            } catch(...) {}

            
            std::stringstream ssOra(ora_str);
            int h, m, s;
            getline(ssOra, d_aux, ':'); h = stoi(d_aux);
            getline(ssOra, d_aux, ':'); m = stoi(d_aux);
            getline(ssOra, d_aux, ':'); s = stoi(d_aux);

            Task t(id, task_content, Data(zi, luna, an), Ora(h, m, s), user_str, priority);
            
            if(modify_task(t))
            {
                send_message(client, "0");
                notify_scheduler();
            }
            else
            {
                send_message(client, "1");
            }
            break;
        }
        default:
            break;
        }
    } catch (const std::exception& e) {
        std::cerr << "[E] handle_request: " << e.what() << "\n";
    }
    return true; 
}



 // sesiune client  
void* session_loop(void* arg) {
    sf::TcpSocket* clientPtr = (sf::TcpSocket*)arg;
    sf::TcpSocket& client = *clientPtr;

    for (;;) {
        std::string msg;
        if (!recv_message(client, msg)) {
            break;
        }
        if (!handle_request(client, msg)) {
            break;
        }
    }

    client.disconnect();
    client.disconnect();
    delete clientPtr;
    return nullptr;
}



void server_main() {

    sf::TcpListener listener;
    if (listener.listen(PORT) != sf::Socket::Done) {
        std::cerr << "Eroare la bind!\n";
        return;
    }
    std::cout << "Server pornit pe portul " << PORT << "...\n";

    for (;;) 
    {
         sf::TcpSocket* client = new sf::TcpSocket();
        auto st = listener.accept(*client);
        if (st != sf::Socket::Done) {
            delete client;
            continue;
        }

        std::cout << "[server] sesiune noua\n";
        pthread_t t;
        if (pthread_create(&t, nullptr, session_loop, client) != 0) {
            perror("pthread_create session");
            delete client;
        } else {
            pthread_detach(t);
        }
    }

    return;
}
