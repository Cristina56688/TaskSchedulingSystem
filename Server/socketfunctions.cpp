#include "header.h"

bool recv_message(sf::TcpSocket& sock, std::string& out) {
    out.clear();
    char c;
    std::size_t n;
    while (true) {
        auto st = sock.receive(&c, 1, n);
        if (st == sf::Socket::Done && n == 1) {
            if (c == '\n') break;
            out += c;
        } else if (st == sf::Socket::Disconnected || st == sf::Socket::Error) {
            return false;
        } else {
            if (out.empty()) return false;
        }
    }
    if (!out.empty()) {
        printf("Am primit: %s\n", out.c_str());
        return true;
    }
    return false;
}

void send_message(sf::TcpSocket& sock, const std::string& s) {
    std::string msg = s + "\n";
    sock.send(msg.c_str(), msg.size());
}