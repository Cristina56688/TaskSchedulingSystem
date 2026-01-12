#include<iostream>
#include<string>
std::string log_in(const std::string& username, const std::string& password)
{
    return "0||" + username + "||" + password + "\n";
}
std::string sign_up(const std::string& username, const std::string& password, const std::string& email)
{
    return "1||" + email + "||" + username+ "||" + password + "\n";
}
std::string log_out()
{
    return "2||\n";
}
std::string add_task(const std::string& task, const std::string& date, const std::string& time, const std::string& username, int priority)
{
    return "3||" + task + "||" + date + "||" + time + "||" + username + "||" + std::to_string(priority) + "\n";
}

std::string get_history(const std::string& username) {
    return "4||" + username + "\n";
}

std::string get_all_tasks(const std::string& username) {
    return "5||" + username + "\n";
}

std::string get_waiting(const std::string& username) {
    return "6||" + username + "\n";
}
std::string modify_task_req(int id, std::string task, std::string data, std::string ora, std::string user, int priority)
{
    return "8||" + std::to_string(id) + "||" + task + "||" + data + "||" + ora + "||" + user + "||" + std::to_string(priority) + "\n";
}

std::string delete_task_req(int id)
{
    return "7||" + std::to_string(id) + "\n";
}
