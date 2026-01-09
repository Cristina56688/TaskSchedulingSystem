#include <QApplication>
#include "MainWindow.h"
#include <SFML/Network.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    sf::TcpSocket socket;

   //std::string ip = "192.168.116.50";
  std::string ip = "127.0.0.1";
    unsigned short port = 54000;

    if (socket.connect(ip, port) != sf::Socket::Done) {
        std::cerr << "Eroare: nu s-a putut conecta la " << ip << ":" << port << "\n";
        return -1;
    }

    std::cout << "Conectat la serverul " << ip << ":" << port << "\n";

    MainWindow window(&socket, ip, port);
    window.show();

    return app.exec();
}
