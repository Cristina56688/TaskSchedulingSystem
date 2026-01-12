#include "MainWindow.h"
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <iostream>
#include<QDebug>
#include "serialize.h"
#include <QMenu>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDialog>
#include <QSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
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
    
    ui.listWidgetTasks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.listWidgetTasks, &QWidget::customContextMenuRequested, this, &MainWindow::on_contextMenuRequested);



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
    std::string temp = msg;
    size_t pos = 0;
    std::string delimiter = "||";

    while ((pos = temp.find(delimiter)) != std::string::npos) {
        tokens.push_back(temp.substr(0, pos));
        temp.erase(0, pos + delimiter.length());
    }
    tokens.push_back(temp); 

    if (tokens.size() < 2)
        return 0;

    std::string type = tokens[0];
    std::string nr_str = tokens[1];
    
    int nr = 0;
    try {
        nr = std::stoi(nr_str);
    } catch (...) {
        return 0;
    }

    QListWidget* targetWidget = ui.listWidgetTasks;
    if (type == "4" || type == "5") {
        targetWidget = ui.listWidgetHistory;
    }

    targetWidget->clear();
    if (nr <= 0) return 0;

    int index = 2;
    int stride = 6; 

    for (int i = 0; i < nr && index + (stride - 1) < (int)tokens.size(); i++) {
        QString afis;
        afis = QString::fromStdString(
            tokens[index] + " | " + 
            tokens[index+1] + " | " + 
            tokens[index+2] + " | " + 
            tokens[index+3] + " | " + 
            tokens[index+4] + " | " + 
            tokens[index+5]
        );
        
        targetWidget->addItem(afis);
        index += stride; 
    }
    return nr;
}


void MainWindow::on_pushButtonWaiting_clicked() {
    drain_socket();
    std::string msg = get_waiting(this->get_username());
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         std::string reply;
         if(receive_message(reply)) {
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
        QMessageBox::warning(this, "Error", "log in data couldn't be sent.");
        return;
    }

    std::string reply;
    if (!receive_message(reply)) {
        QMessageBox::warning(this, "Error", "Couldn't get response from server.");
        return;
    }

    std::cout<<"Server response for log:"<<reply<<std::endl;
    if(reply[0] == '2') { 
 
        set_username(user);
        parse_tasks(reply);
        ui.stackedWidget->setCurrentWidget(ui.mainpage);
    } else {
        QMessageBox::warning(
            this,
            "Error",
            "Check the username and password."
           
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
        QMessageBox::warning(this, "Error", "sign in data couldn't be sent.");
        return;
    }

    std::string reply;
    if (!receive_message(reply)) {
        QMessageBox::warning(this, "Error", "Couldn't get response from server.");
        return;
    }
    
    if (reply == "0") {
        QMessageBox::information(this, "Success", "Account created! You can log in.");
        ui.stackedWidget->setCurrentWidget(ui.login);
    } else {
        QMessageBox::warning(this, "Error", "Account creation failed. The user already exists.");
    }
}

void MainWindow::on_pushButtonSendTask_clicked()
{
    drain_socket();
    std::string task = ui.lineEdit_3->text().toStdString();

    QDate date = ui.calendarWidget->selectedDate();
    QTime time = ui.timeEdit->time();
    std::string data = date.toString("dd.MM.yyyy").toStdString();
    std::string ora = time.toString("HH:mm:ss").toStdString();
    
    int priority = ui.spinBoxPriority->value();
    
    std::string mesaj = add_task(task, data, ora, this->get_username(), priority);
    std::cout<<mesaj<<std::endl;
    if (socket->send(mesaj.c_str(), mesaj.size()) != sf::Socket::Done) {
        QMessageBox::warning(this, "Error", "Couldn't send the task.");
        return;
    }

    std::string reply;
    receive_message(reply); 
}
void MainWindow::on_pushButtonLogOut_clicked()
{
    drain_socket();
    std::string mesaj = log_out();
    if (socket->send(mesaj.c_str(), mesaj.size()) != sf::Socket::Done) {
        QMessageBox::warning(this, "Error", "log out request couldn't be sent.");
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
    drain_socket();
    std::string msg = get_history(this->get_username());
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         std::string reply;
         if(receive_message(reply)) {
             parse_tasks(reply);
         }
    }
}

void MainWindow::on_pushButtonAll_clicked() {
    drain_socket();
    std::string msg = get_all_tasks(this->get_username());
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         std::string reply;
         if(receive_message(reply)) {
             parse_tasks(reply);
         }
    }
}

void MainWindow::on_contextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui.listWidgetTasks->itemAt(pos);
    if (!item) return;

    QMenu contextMenu(tr("Context menu"), this);

    QAction actionDelete("Delete Task", this);
    connect(&actionDelete, &QAction::triggered, this, &MainWindow::delete_current_task);
    contextMenu.addAction(&actionDelete);

    QAction actionModify("Modify Task", this);
    connect(&actionModify, &QAction::triggered, this, &MainWindow::modify_current_task);
    contextMenu.addAction(&actionModify);

    contextMenu.exec(ui.listWidgetTasks->mapToGlobal(pos));
}

void MainWindow::delete_current_task()
{
    QListWidgetItem *item = ui.listWidgetTasks->currentItem();
    if(!item) return;

    QString text = item->text();
    QStringList parts = text.split('|');
    if(parts.size() < 1) return;

    int id = parts[0].trimmed().toInt();

    std::string msg = delete_task_req(id);
    if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
         std::string reply;
         if(receive_message(reply)) {
             if(reply == "0") {
            
                 on_pushButtonWaiting_clicked(); 
             } else {
                 QMessageBox::warning(this, "Error", "Failed to delete task.");
             }
         }
    }
}

void MainWindow::modify_current_task()
{
    QListWidgetItem *item = ui.listWidgetTasks->currentItem();
    if(!item) return;

    QString text = item->text();
    QStringList parts = text.split('|');
    if(parts.size() < 3) return;

    int id = parts[0].trimmed().toInt();
    QString currentDesc = parts[1].trimmed();
    QString dateTimeStr = parts[2].trimmed();
    
    QDateTime dt = QDateTime::fromString(dateTimeStr, "d.M.yyyy H:m:s");

    QDialog dialog(this);
    dialog.setWindowTitle("Modify Task");
    QFormLayout form(&dialog);

    QLineEdit *descEdit = new QLineEdit(&dialog);
    descEdit->setText(currentDesc);
    form.addRow("Task:", descEdit);

    QDateEdit *dateEdit = new QDateEdit(&dialog);
    dateEdit->setDate(dt.date());
    form.addRow("Date:", dateEdit);

    QTimeEdit *timeEdit = new QTimeEdit(&dialog);
    timeEdit->setTime(dt.time());
    form.addRow("Time:", timeEdit);

    QSpinBox *prioBox = new QSpinBox(&dialog);
    prioBox->setRange(1, 10);
    prioBox->setValue(1);
    form.addRow("Priority:", prioBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        std::string task = descEdit->text().toStdString();
        std::string data = dateEdit->date().toString("dd.MM.yyyy").toStdString();
        std::string ora = timeEdit->time().toString("HH:mm:ss").toStdString();
        int priority = prioBox->value();

        std::string msg = modify_task_req(id, task, data, ora, this->get_username(), priority);
        
        if(socket->send(msg.c_str(), msg.size()) == sf::Socket::Done) {
             std::string reply;
             if(receive_message(reply)) {
                 if(reply == "0") {
            
                     on_pushButtonWaiting_clicked(); 
                 } else {
                     QMessageBox::warning(this, "Error", "Failed to modify task.");
                 }
             }
        }
    }
}

bool MainWindow::receive_message(std::string &out) {
    out.clear();
    char c;
    std::size_t n;
    
    sf::SocketSelector selector;
    selector.add(*socket);

    while (true) {
        if (selector.wait(sf::seconds(3))) {
            auto st = socket->receive(&c, 1, n);
            if (st == sf::Socket::Done && n == 1) {
                if (c == '\n') break;
                out += c;
            } else if (st == sf::Socket::Disconnected || st == sf::Socket::Error) {
                return false;
            } else {
                 if(st == sf::Socket::NotReady) continue;
                 return false;
            }
        } else {
            return false;
        }
    }
    return !out.empty();
}

bool MainWindow::drain_socket() {
    socket->setBlocking(false);
    char d;
    std::size_t received;
    while (socket->receive(&d, 1, received) == sf::Socket::Done) {
    }
    socket->setBlocking(true);
    return true;
}
