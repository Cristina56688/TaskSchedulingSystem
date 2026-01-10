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
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <cstring>

#include <map>
#include <deque>

using namespace tinyxml2;

#define SENDER_EMAIL "cristinaghinoiu40@gmail.com"
#define SENDER_PASSWORD "zoil jtcx xitr enwe"


struct ExecTask {
    int id;
    std::string command;
    std::string user;

    int priority;
    long long timestamp;

    pid_t pid;          
    bool suspended;     
    long long total_duration_ms;

    ExecTask() : id(-1), command(""), user(""), priority(1), timestamp(0), pid(0), suspended(false), total_duration_ms(0) {}

    bool operator<(const ExecTask& other) const {
        if (priority != other.priority) {
            return priority < other.priority; 
        }
        return timestamp > other.timestamp; 
    }
};


class Mutex {
private:
    pthread_mutex_t m_mutex;
public:
    Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }
    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }
    void lock() {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
    pthread_mutex_t* native_handle() {
        return &m_mutex;
    }
};

class LockGuard {
private:
    Mutex& m_mutex;
public:
    LockGuard(Mutex& m) : m_mutex(m) {
        m_mutex.lock();
    }
    ~LockGuard() {
        m_mutex.unlock();
    }
};

class ConditionVariable {
private:
    pthread_cond_t m_cond;
public:
    ConditionVariable() {
        pthread_cond_init(&m_cond, nullptr);
    }
    ~ConditionVariable() {
        pthread_cond_destroy(&m_cond);
    }
    void wait(Mutex& mutex) {
        pthread_cond_wait(&m_cond, mutex.native_handle());
    }
    
    template<typename Predicate>
    void wait(Mutex& mutex, Predicate pred) {
        while (!pred()) {
            wait(mutex);
        }
    }
    void notify_one() {
        pthread_cond_signal(&m_cond);
    }
    void notify_all() {
        pthread_cond_broadcast(&m_cond);
    }
};

// resurse shared
extern std::priority_queue<ExecTask> taskQueue;
extern std::map<std::string, std::deque<ExecTask>> userQueues; 
extern std::vector<std::string> userList;                     
extern int currentUserIndex;                                  
extern const int TIME_QUANTUM_MS;                             

extern Mutex queueMutex;
extern ConditionVariable queueCond;
extern std::atomic<bool> reload_needed;

// Thread functions
void* run_scheduler(void* arg);
void* run_executor(void* arg);
void server_main();

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
    int _priority;
public:
    Task():_data(), _ora(), _user(""), _id(-1), _priority(1){};
    Task(std::string str);
    Task(const int id, const std::string task, const Data data, const Ora ora, const std::string user, const int priority=1);
    std::vector<std::string> getTask() const {return _task;}
    Data getData() const {return _data;}
    Ora getOra() const {return _ora;}
    std::string getUserName() const {return _user;}
    int getId() const { return _id; } 
    int getPriority() const { return _priority; }
    void setPriority(int p) { _priority = p; }
};

//Account Manager
bool accountExists(std::vector<std::pair<std::string, std::string>> existing, std::string userName, std::string password);
bool addAccount(const std::string& filePath, const SingInData& sd);
bool verifyAccount(const std::string& filePath, const LogInData& ld);
std::string getMailByUserName(const std::string& userName);


//Mail Manager
void sendAccountCreationConfirmation(const std::string mail);
void sendTaskFailureNotification(const std::string mail, int taskId, int exitCode, const std::string& command);
void sendAbnormalDurationNotification(const std::string mail, int taskId, long long duration_ms, const std::string& command);


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
std::string create_message_history(const std::string username);
std::string create_message_all(const std::string username);
std::string create_message_waiting(const std::string username);
bool remove_task(const int id);
bool modify_task(const Task& task);

// Runners
void run_scheduler();
void run_executor();

// History
void log_history(int id, const std::string& cmd, int exit_code, const std::string& user, long long duration_ms);

#endif