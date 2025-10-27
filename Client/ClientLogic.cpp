#include "ClientLogic.h"
#include <fstream>
#include <iostream>

ClientLogic::ClientLogic(const std::string& ip, unsigned short port, const std::string& iface)
    : serverIp(ip), port(port), iface(iface) {}

std::string ClientLogic::getMac() const {
    std::string path = "/sys/class/net/" + iface + "/address";
    std::ifstream file(path);
    std::string mac;
    if (file.is_open()) {
        file >> mac;
    } else {
        mac = "eroare";
    }
    return mac;
}

bool ClientLogic::sendMessage(const std::string& message) {
    sf::TcpSocket socket;

    if (socket.connect(serverIp, port, sf::seconds(5)) != sf::Socket::Done) {
        std::cerr << "eroare la conectare cu server\n";
        return false;
    }

    if (socket.send(message.c_str(), message.size()) != sf::Socket::Done) {
        std::cerr << "eroare trimitere mesaj!\n";
        return false;
    }

    std::cout << "mesaj trimis : " << message << "\n";
    socket.disconnect();
    return true;
}
