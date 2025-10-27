#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PasswordDialog.h"
#include <QMessageBox>
#include "MessageFactory.h" 
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      client("192.168.79.101", 54000, "eth0")  
{
    ui->setupUi(this);
    connect(ui->pushButtonSubmit, &QPushButton::clicked, this, &MainWindow::onSubmitClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSubmitClicked() {
    PasswordDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    QString password = dlg.getPassword();
    std::string mac = client.getMac();

    std::string message = MessageFactory::mesaj_autentificare(mac, password.toStdString());

    std::cout<< message<<std::endl;

    bool ok = client.sendMessage(message);

    if (ok)
        QMessageBox::information(this, "Succes", "MAC si parola trimise\n");
    else
        QMessageBox::warning(this, "Error", "nu s-a putut trimite mesajul\n");
}
