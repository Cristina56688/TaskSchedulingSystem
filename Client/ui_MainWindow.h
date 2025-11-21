/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *welcome;
    QLabel *label;
    QPushButton *pushButtonlogin;
    QPushButton *pushButtonsignup;
    QWidget *login;
    QPushButton *pushButtonSendlog;
    QLabel *labelLogIn;
    QLineEdit *lineEditUsernameLog;
    QLineEdit *lineEditPasswordLog;
    QLabel *labelUsernameLog;
    QLabel *labelPasswordLog;
    QPushButton *pushButtonsigninacc;
    QLabel *labelacc;
    QWidget *signup;
    QLineEdit *lineEditMailRegister;
    QLineEdit *lineEditUsernameRegister_2;
    QLineEdit *lineEditPasswordRegister;
    QLabel *labelMail;
    QLabel *labelUsernameReg;
    QLabel *labelPasswordReg;
    QLabel *labelRegister;
    QPushButton *pushButtonSendSign;
    QLabel *label_2;
    QPushButton *pushButtonlogacc;
    QWidget *mainpage;
    QFormLayout *formLayout;
    QTimeEdit *timeEdit;
    QLabel *labelTime;
    QCalendarWidget *calendarWidget;
    QLabel *labelDate;
    QLineEdit *lineEdit_3;
    QLabel *labelTask;
    QPushButton *pushButtonSendTask;
    QPushButton *pushButtonLogOut;
    QListWidget *listWidgetTasks;
    QLabel *labelTasksTitle;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        welcome = new QWidget();
        welcome->setObjectName(QString::fromUtf8("welcome"));
        label = new QLabel(welcome);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 50, 432, 43));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu Sans Mono"));
        font.setPointSize(26);
        label->setFont(font);
        pushButtonlogin = new QPushButton(welcome);
        pushButtonlogin->setObjectName(QString::fromUtf8("pushButtonlogin"));
        pushButtonlogin->setGeometry(QRect(220, 210, 291, 61));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font1.setPointSize(14);
        pushButtonlogin->setFont(font1);
        pushButtonsignup = new QPushButton(welcome);
        pushButtonsignup->setObjectName(QString::fromUtf8("pushButtonsignup"));
        pushButtonsignup->setGeometry(QRect(220, 330, 291, 61));
        pushButtonsignup->setFont(font1);
        stackedWidget->addWidget(welcome);
        login = new QWidget();
        login->setObjectName(QString::fromUtf8("login"));
        pushButtonSendlog = new QPushButton(login);
        pushButtonSendlog->setObjectName(QString::fromUtf8("pushButtonSendlog"));
        pushButtonSendlog->setGeometry(QRect(230, 350, 251, 61));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font2.setPointSize(12);
        font2.setItalic(true);
        pushButtonSendlog->setFont(font2);
        labelLogIn = new QLabel(login);
        labelLogIn->setObjectName(QString::fromUtf8("labelLogIn"));
        labelLogIn->setGeometry(QRect(310, 40, 151, 61));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Ubuntu Sans Mono"));
        font3.setPointSize(22);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        labelLogIn->setFont(font3);
        labelLogIn->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEditUsernameLog = new QLineEdit(login);
        lineEditUsernameLog->setObjectName(QString::fromUtf8("lineEditUsernameLog"));
        lineEditUsernameLog->setGeometry(QRect(230, 180, 251, 31));
        lineEditPasswordLog = new QLineEdit(login);
        lineEditPasswordLog->setObjectName(QString::fromUtf8("lineEditPasswordLog"));
        lineEditPasswordLog->setGeometry(QRect(230, 270, 251, 31));
        labelUsernameLog = new QLabel(login);
        labelUsernameLog->setObjectName(QString::fromUtf8("labelUsernameLog"));
        labelUsernameLog->setGeometry(QRect(230, 140, 201, 31));
        labelUsernameLog->setFont(font2);
        labelPasswordLog = new QLabel(login);
        labelPasswordLog->setObjectName(QString::fromUtf8("labelPasswordLog"));
        labelPasswordLog->setGeometry(QRect(230, 230, 201, 31));
        labelPasswordLog->setFont(font2);
        pushButtonsigninacc = new QPushButton(login);
        pushButtonsigninacc->setObjectName(QString::fromUtf8("pushButtonsigninacc"));
        pushButtonsigninacc->setGeometry(QRect(490, 430, 101, 41));
        labelacc = new QLabel(login);
        labelacc->setObjectName(QString::fromUtf8("labelacc"));
        labelacc->setGeometry(QRect(230, 430, 221, 41));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Ubuntu Sans Mono"));
        font4.setPointSize(12);
        font4.setItalic(true);
        labelacc->setFont(font4);
        stackedWidget->addWidget(login);
        signup = new QWidget();
        signup->setObjectName(QString::fromUtf8("signup"));
        lineEditMailRegister = new QLineEdit(signup);
        lineEditMailRegister->setObjectName(QString::fromUtf8("lineEditMailRegister"));
        lineEditMailRegister->setGeometry(QRect(270, 160, 251, 31));
        lineEditUsernameRegister_2 = new QLineEdit(signup);
        lineEditUsernameRegister_2->setObjectName(QString::fromUtf8("lineEditUsernameRegister_2"));
        lineEditUsernameRegister_2->setGeometry(QRect(270, 240, 251, 31));
        lineEditPasswordRegister = new QLineEdit(signup);
        lineEditPasswordRegister->setObjectName(QString::fromUtf8("lineEditPasswordRegister"));
        lineEditPasswordRegister->setGeometry(QRect(270, 320, 251, 31));
        labelMail = new QLabel(signup);
        labelMail->setObjectName(QString::fromUtf8("labelMail"));
        labelMail->setGeometry(QRect(270, 120, 201, 31));
        labelMail->setFont(font2);
        labelUsernameReg = new QLabel(signup);
        labelUsernameReg->setObjectName(QString::fromUtf8("labelUsernameReg"));
        labelUsernameReg->setGeometry(QRect(270, 200, 201, 31));
        labelUsernameReg->setFont(font2);
        labelPasswordReg = new QLabel(signup);
        labelPasswordReg->setObjectName(QString::fromUtf8("labelPasswordReg"));
        labelPasswordReg->setGeometry(QRect(270, 280, 201, 31));
        labelPasswordReg->setFont(font2);
        labelRegister = new QLabel(signup);
        labelRegister->setObjectName(QString::fromUtf8("labelRegister"));
        labelRegister->setGeometry(QRect(310, 20, 151, 61));
        labelRegister->setFont(font3);
        labelRegister->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pushButtonSendSign = new QPushButton(signup);
        pushButtonSendSign->setObjectName(QString::fromUtf8("pushButtonSendSign"));
        pushButtonSendSign->setGeometry(QRect(270, 390, 251, 61));
        pushButtonSendSign->setFont(font2);
        label_2 = new QLabel(signup);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 480, 211, 41));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Ubuntu"));
        font5.setPointSize(12);
        font5.setItalic(true);
        label_2->setFont(font5);
        pushButtonlogacc = new QPushButton(signup);
        pushButtonlogacc->setObjectName(QString::fromUtf8("pushButtonlogacc"));
        pushButtonlogacc->setGeometry(QRect(210, 480, 91, 31));
        stackedWidget->addWidget(signup);
        mainpage = new QWidget();
        mainpage->setObjectName(QString::fromUtf8("mainpage"));
        formLayout = new QFormLayout(mainpage);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        timeEdit = new QTimeEdit(mainpage);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setFont(font1);

        formLayout->setWidget(4, QFormLayout::FieldRole, timeEdit);

        labelTime = new QLabel(mainpage);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font6.setPointSize(14);
        font6.setItalic(true);
        labelTime->setFont(font6);

        formLayout->setWidget(4, QFormLayout::LabelRole, labelTime);

        calendarWidget = new QCalendarWidget(mainpage);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
        QFont font7;
        font7.setPointSize(10);
        calendarWidget->setFont(font7);

        formLayout->setWidget(3, QFormLayout::FieldRole, calendarWidget);

        labelDate = new QLabel(mainpage);
        labelDate->setObjectName(QString::fromUtf8("labelDate"));
        labelDate->setFont(font6);

        formLayout->setWidget(3, QFormLayout::LabelRole, labelDate);

        lineEdit_3 = new QLineEdit(mainpage);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_3);

        labelTask = new QLabel(mainpage);
        labelTask->setObjectName(QString::fromUtf8("labelTask"));
        labelTask->setFont(font6);

        formLayout->setWidget(1, QFormLayout::LabelRole, labelTask);

        pushButtonSendTask = new QPushButton(mainpage);
        pushButtonSendTask->setObjectName(QString::fromUtf8("pushButtonSendTask"));
        pushButtonSendTask->setFont(font6);

        formLayout->setWidget(5, QFormLayout::FieldRole, pushButtonSendTask);

        pushButtonLogOut = new QPushButton(mainpage);
        pushButtonLogOut->setObjectName(QString::fromUtf8("pushButtonLogOut"));
        pushButtonLogOut->setFont(font2);

        formLayout->setWidget(0, QFormLayout::LabelRole, pushButtonLogOut);

        listWidgetTasks = new QListWidget(mainpage);
        listWidgetTasks->setObjectName(QString::fromUtf8("listWidgetTasks"));

        formLayout->setWidget(6, QFormLayout::FieldRole, listWidgetTasks);

        labelTasksTitle = new QLabel(mainpage);
        labelTasksTitle->setObjectName(QString::fromUtf8("labelTasksTitle"));
        labelTasksTitle->setFont(font6);

        formLayout->setWidget(6, QFormLayout::LabelRole, labelTasksTitle);

        stackedWidget->addWidget(mainpage);

        horizontalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Task Scheduling System", nullptr));
        pushButtonlogin->setText(QCoreApplication::translate("MainWindow", "Log in", nullptr));
        pushButtonsignup->setText(QCoreApplication::translate("MainWindow", "Sign up", nullptr));
        pushButtonSendlog->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        labelLogIn->setText(QCoreApplication::translate("MainWindow", "Log in ", nullptr));
        labelUsernameLog->setText(QCoreApplication::translate("MainWindow", "Username:", nullptr));
        labelPasswordLog->setText(QCoreApplication::translate("MainWindow", "Password:", nullptr));
        pushButtonsigninacc->setText(QCoreApplication::translate("MainWindow", "Sign up", nullptr));
        labelacc->setText(QCoreApplication::translate("MainWindow", "I don't have an account", nullptr));
        labelMail->setText(QCoreApplication::translate("MainWindow", "Email Address", nullptr));
        labelUsernameReg->setText(QCoreApplication::translate("MainWindow", "Username:", nullptr));
        labelPasswordReg->setText(QCoreApplication::translate("MainWindow", "Password:", nullptr));
        labelRegister->setText(QCoreApplication::translate("MainWindow", "Sign Up", nullptr));
        pushButtonSendSign->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "I already have an account", nullptr));
        pushButtonlogacc->setText(QCoreApplication::translate("MainWindow", "Log in", nullptr));
        labelTime->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        labelDate->setText(QCoreApplication::translate("MainWindow", "Date:", nullptr));
        labelTask->setText(QCoreApplication::translate("MainWindow", "Enter your task:", nullptr));
        pushButtonSendTask->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        pushButtonLogOut->setText(QCoreApplication::translate("MainWindow", "Log Out", nullptr));
        labelTasksTitle->setText(QCoreApplication::translate("MainWindow", "Programmed Tasks:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
