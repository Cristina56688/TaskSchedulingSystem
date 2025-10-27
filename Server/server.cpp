#include <SFML/Network.hpp>
#include <iostream>
#include "header.h"

#define MACPASSF "macpasswordlist.xml"

int getIndex(std::string str)
{
    std::istringstream ss(str);
    std::string index;
    std::getline(ss, index, '|');

    return std::stoi(index);
}

int main() 
{
    std::string macPassFile = MACPASSF;

    sf::TcpListener listener;

    if (listener.listen(54000) != sf::Socket::Done) {
        std::cout << "Eroare la bind!\n";
        return -1;
    }

    std::cout << "Server pornit pe portul 54000...\n";

    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done) {
        std::cout << "Eroare la accept!\n";
        return -1;
    }

    std::cout << "Client conectat!\n";

    while (true) {
        char data[1024];
        std::size_t received;
        if (client.receive(data, sizeof(data), received) == sf::Socket::Done) {
            std::cout << "Am primit: " << std::string(data, received) << "\n";

            int index=getIndex(data);

            switch(index) 
                case 0:
                {
                    MacPass mp(data);

                        try {
                            addMacPassword(macPassFile, mp.getMac(), mp.getPassword());
                        } catch (const std::exception& e) {
                            std::cerr << "Exceptie: " << e.what() << "\n";
                        }
                }

            std::string reply = "Server: Task primit!\n";
            client.send(reply.c_str(), reply.size());
        }
    }

    return 0;
}
