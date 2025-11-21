#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <tinyxml2.h>
#include <iostream>
#include <regex>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>
#include <SFML/Network.hpp>
#include <csignal>
#include <cerrno>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <random>
#include <limits>
#include <atomic>
#include <error.h>


using namespace tinyxml2;

class SingInData
{
private:
    std::string _mail;
	std::string _userName;
	std::string _password;

public:
	SingInData() :_mail(""), _userName(""), _password("") {};
	SingInData(const std::string str);
	std::string getUserName() const { return _userName; };
	std::string getPassword() const { return _password; }
    std::string getMail() const { return _mail; }

};

class LogInData
{
private:
    std::string _userName;
    std::string _password;

public:
    LogInData() :_userName(""), _password("") {};
    LogInData(const std::string str);
    std::string getUserName() const  { return _userName; };
    std::string getPassword() const { return _password; }
};

class Ora
{
private:
    int _ora;
    int _min;
    int _sec;
public:
    Ora():_ora(0), _min(0), _sec(0){}
    Ora(const int& o, const int& m, const int& s):_ora(o), _min(m), _sec(s){}
    int getOra() const {return _ora;}
    int getMin() const {return _min;}
    int getSec() const {return _sec;}
    void setOra(const int& x) {_ora=x;}
    void setMin(const int& x) {_min=x;}
    void setSec(const int& x) {_sec=x;}
};

class Data
{
private:
    int _zi;
    int _luna;
    int _an;
public:
    Data():_zi(0), _luna(0), _an(0){}
    Data(const int& z, const int& l, const int& a):_zi(z), _luna(l), _an(a){}
    int getZi() const {return _zi;}
    int getLuna() const {return _luna;}
    int getAn() const {return _an;}
    void setZi(const int& x) {_zi=x;}
    void setLuna(const int& x) {_luna=x;}
    void setAn(const int& x) {_an=x;}
};

class Task
{
private:
    std::vector<std::string> _task;
    Data _data;  
    Ora _ora; 
    std::string _user;
    int _id;
public:
    Task():_data(), _ora(), _user(""), _id(-1){};
    Task(std::string str);
    Task(const int id, const std::string task, const Data data, const Ora ora, const std::string user);
    std::vector<std::string> getTask() const {return _task;}
    Data getData() const {return _data;}
    Ora getOra() const {return _ora;}
    std::string getUserName() const {return _user;}
};

//Acount Manager
bool accountExists(std::vector<std::pair<std::string, std::string>> existing, std::string userName, std::string password);
bool addAccount(const std::string& filePath, const SingInData& sd);
bool verifyAccount(const std::string& filePath, const LogInData& ld);

//Mail Manager
void sendAccountCreationConfirmation(const std::string mail);

//Utils
int getIndex(const std::string& str);
std::vector<std::string> split(const std::string& s);

//Socket functions
bool recv_message(sf::TcpSocket& sock, std::string& out);
void send_message(sf::TcpSocket& sock, const std::string& s);

//Task Manager
bool save_task(const std::string& filePath, const Task& task);
std::vector<Task> extract_waiting_tasks();
std::string create_message_login(const std::string username);

#endif