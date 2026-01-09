#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <string>       
std::string log_in(const std::string& username, const std::string& password);
std::string sign_up(const std::string& username, const std::string& password, const std::string& email);
std::string add_task(const std::string& task, const std::string& date, const std::string& time, const std::string& username, int priority = 1);
std::string get_history(const std::string& username);
std::string get_all_tasks(const std::string& username);
std::string get_waiting(const std::string& username);
std::string log_out();
#endif