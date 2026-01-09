#include "MainWindow.h"
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <iostream>
#include<QDebug>
#include "serialize.h"
MainWindow::MainWindow(sf::TcpSocket *sock, const std::string& ip, unsigned short port, QWidget *parent)
    : QMainWindow(parent), socket(sock), serverIp(ip), serverPort(port)
{
    ui.setupUi(this);
   this->setStyleSheet("QMainWindow {"
                        "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "                             stop: 0 #ADD8E6, stop: 1 #4682B4);" 
                        "}"
                        

                        "QPushButton {"
                        "    background-color: #bdd2e4ff;" 
                        "    border: 1px solid #12028dff;" 
                        "    padding: 2px;" 
                        "}");

    ui.pushButtonSendlog->setAutoDefault(false);
ui.pushButtonSendlog->setDefault(false);


    ui.stackedWidget->setCurrentWidget(ui.welcome);

}


MainWindow::~MainWindow() {}

void MainWindow::checkConnection() {

     if (socket->getRemoteAddress() == sf::IpAddress::None) {
         std::cout << "Reconnecting to " << serverIp << ":" << serverPort << "...\n";
         socket->disconnect();
         if (socket->connect(serverIp, serverPort) != sf::Socket::Done) {
             std::cerr << "Failed to reconnect!\n";
             QMessageBox::warning(this, "Connection Error", "Could not reconnect to server!");
         } else {
             std::cout << "Reconnected successfully.\n";
         }
     }
}


void MainWindow::on_pushButtonlogin_clicked()
{
    ui.stackedWidget->setCurrentWidget(ui.login);

}

void MainWindow::on_pushButtonsignup_clicked()
{
    ui.stackedWidget->setCurrentWidget(ui.signup);
}
int MainWindow::parse_tasks(const std::string &msg)
{
    std::vector<std::string> tokens;
    std::stringstream ss(msg);
    std::string part;

    while (std::getline(ss, part, '|')) {
        if (!part.empty()) {
            tokens.push_back(part);
        }
    }

    if (tokens.size() < 2)
        return 0;
    std::string nr_str = tokens[1];
    bool e_numar = true;
    for (char c : nr_str) {
        if (!std::isdigit(c)) {
            e_numar = false;
            break;
        }
    }

    int nr = e_numar ? std::stoi(nr_str) : 0;

    QListWidget* targetWidget = ui.listWidgetTasks;
    if (tokens[0] == "4" || tokens[0] == "5") {
        targetWidget = ui.listWidgetHistory;
    }

    if (nr == 0) {
        targetWidget->clear();
        return 0;
    }
    
    targetWidget->clear();

    int index = 2;
    int stride = 3;
    if (tokens[0] == "4" || tokens[0] == "5" || tokens[0] == "6") stride = 4;

    for (int i = 0; i < nr && index + (stride - 1) < (int)tokens.size(); i++) {
        QString afis;
        if (tokens[0] == "2") {
            afis = QString::fromStdString(tokens[index] + " | " + tokens[index+1] + " | " + tokens[index+2]);
            index += stride; 
        } 
        else if (tokens[0] == "6") {
             afis = QString::fromStdString(tokens[index+1] + " | " + tokens[index+2] + " | " + tokens[index+3]);
             index += stride; 
        }
        else if (tokens[0] == "4" || tokens[0] == "5") {
            afis = QString::fromStdString(tokens[index+1] + " | " + tokens[index+2] + " | " + tokens[index+3]);
            index += stride; 
        }
        targetWidget->addItem(afis);
    }
    return nr;
}


void MainWindow::on_pushButtonWaiting_clicked() {
    std::string msg = get_waiting(this->get_username());
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         char buffer[8192];
         size_t received;
         if(socket->receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
             std::string reply(buffer, received);
             parse_tasks(reply);
         }
    }
}

void MainWindow::on_pushButtonSendlog_clicked()
{
    std::string user = ui.lineEditUsernameLog->text().toStdString();
    std::string pass = ui.lineEditPasswordLog->text().toStdString();

    checkConnection();

    std::string mesaj = log_in(user, pass);

    if (socket->send(mesaj.c_str(), mesaj.size()) != sf::Socket::Done) {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut trimite datele de autentificare.");
        return;
    }

    char buffer[4096]; 
    std::size_t received = 0;

    if (socket->receive(buffer, sizeof(buffer), received) != sf::Socket::Done) {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut primi răspunsul de la server.");
        return;
    }

    std::string reply(buffer, received);
    std::cout<<"Raspuns de la server pentru log:"<<reply<<std::endl;
    if(reply[0] == '2') { 
 
        set_username(user);
        parse_tasks(reply);
        ui.stackedWidget->setCurrentWidget(ui.mainpage);
    } else {
        QMessageBox::warning(
            this,
            "Eroare",
            "Autentificare eșuată. Verifică username-ul și parola."
           
        );
    }

}


void MainWindow::on_pushButtonSendSign_clicked()
{
    std::string email = ui.lineEditMailRegister->text().toStdString();
    std::string user = ui.lineEditUsernameRegister_2->text().toStdString();
    std::string pass = ui.lineEditPasswordRegister->text().toStdString();
    
    checkConnection();

    std::string mesaj = sign_up(user, pass, email);

    if (socket->send(mesaj.c_str(), mesaj.size()) != sf::Socket::Done) {
        QMessageBox::warning(this, "Eroare", "Nu s-au putut trimite datele de inregistrare.");
        return;
    }

    char buffer[1024];
    std::size_t received = 0;
    if (socket->receive(buffer, sizeof(buffer), received) != sf::Socket::Done) {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut primi răspunsul de la server.");
        return;
    }
    
    std::string reply(buffer, received);
    if (reply == "0") {
        QMessageBox::information(this, "Succes", "Cont creat cu succes! Te poti autentifica.");
        ui.stackedWidget->setCurrentWidget(ui.login);
    } else {
        QMessageBox::warning(this, "Eroare", "Crearea contului a esuat. Posibil user existent.");
    }
}

void MainWindow::on_pushButtonSendTask_clicked()
{

    std::string task = ui.lineEdit_3->text().toStdString();

    QDate date = ui.calendarWidget->selectedDate();
    QTime time = ui.timeEdit->time();
    std::string data = date.toString("dd.MM.yyyy").toStdString();
    std::string ora = time.toString("HH:mm:ss").toStdString();
    
    int priority = ui.spinBoxPriority->value();
    
    std::string mesaj = add_task(task, data, ora, this->get_username(), priority);
    std::cout<<mesaj<<std::endl;
    if (socket->send(mesaj.c_str(), mesaj.size()) != sf::Socket::Done) {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut trimite task ul.");
        return;
    }

}
void MainWindow::on_pushButtonLogOut_clicked()
{
    std::string mesaj = log_out();
    if (socket->send(mesaj.c_str(), mesaj.size()) != sf::Socket::Done) {
        QMessageBox::warning(this, "Eroare", "Nu s-a putut trimite cererea de logout.");
        return;
    }
    ui.stackedWidget->setCurrentWidget(ui.welcome);
    socket->disconnect();
    this->set_username("");
}
void MainWindow::on_pushButtonlogacc_clicked()
{
      ui.stackedWidget->setCurrentWidget(ui.login);
}
void MainWindow::on_pushButtonsigninacc_clicked()
{
  ui.stackedWidget->setCurrentWidget(ui.signup);
}



void MainWindow::on_pushButtonHistory_clicked() {
    std::string msg = get_history(this->get_username());
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         char buffer[8192];
         size_t received;
         if(socket->receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
             std::string reply(buffer, received);
             parse_tasks(reply);
         }
    }
}

void MainWindow::on_pushButtonAll_clicked() {
    std::string msg = get_all_tasks(this->get_username());
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         char buffer[8192];
         size_t received;
         if(socket->receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
             std::string reply(buffer, received);
             parse_tasks(reply);
         }
    }
}
