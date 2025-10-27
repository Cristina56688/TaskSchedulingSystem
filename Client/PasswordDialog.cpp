#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &PasswordDialog::onSendClicked);
}

PasswordDialog::~PasswordDialog() {
    delete ui;
}

void PasswordDialog::onSendClicked() {
    password = ui->lineEdit->text();
    if (password.isEmpty()) {
        ui->label->setText("Please enter your password!");
        return;
    }
    accept();
}

QString PasswordDialog::getPassword() const {
    return password;
}
