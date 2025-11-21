#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h"
#include <SFML/Network.hpp>
#include <sstream>
#include <vector>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(sf::TcpSocket *socket, QWidget *parent = nullptr);
    void set_username(const std::string& username) { loggged_in_user = username; }
    std::string get_username() const { return loggged_in_user; }
      int parse_tasks(const std::string &msg);
    ~MainWindow();
private slots:
    void on_pushButtonlogin_clicked();      
    void on_pushButtonsignup_clicked();     
    void on_pushButtonSendlog_clicked();  
    void on_pushButtonSendSign_clicked();  
    void on_pushButtonSendTask_clicked();  
    void on_pushButtonLogOut_clicked();
    void on_pushButtonlogacc_clicked();
    void on_pushButtonsigninacc_clicked();

private:
    Ui::MainWindow ui;
    sf::TcpSocket *socket; 
    std::string loggged_in_user;
};

#endif // MAINWINDOW_H
