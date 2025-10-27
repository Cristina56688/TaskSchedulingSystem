#ifndef CLIENT_LOGIC_H
#define CLIENT_LOGIC_H

#include <SFML/Network.hpp>
#include <string>

class ClientLogic {
private:
    std::string serverIp;
    unsigned short port;
    std::string iface;

public:
    ClientLogic(const std::string& ip, unsigned short port, const std::string& iface = "eth0");
    std::string getMac() const;
    bool sendMessage(const std::string& message);
};

#endif
