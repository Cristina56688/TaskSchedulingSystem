#include "header.h"

bool recv_message(sf::TcpSocket& sock, std::string& out) {
    char buf[2048];
    std::size_t n = 0;
    auto st = sock.receive(buf, sizeof(buf), n);
    if (st == sf::Socket::Done && n > 0) {

        buf[n]='\0';
        std::string aux(buf);
        out=aux;
        printf("Am primit: %s\n", out.c_str());
        return true;
        
    }
    return false; 
}


void send_message(sf::TcpSocket& sock, const std::string& s) {
    sock.send(s.c_str(), s.size());
}