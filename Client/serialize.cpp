#include<iostream>
#include<string>
std::string log_in(const std::string& username, const std::string& password)
{
    return "0||" + username + "||" + password + "\0";
}
std::string sign_up(const std::string& username, const std::string& password, const std::string& email)
{
    return "1||" + email + "||" + username+ "||" + password + "\0";
}
std::string log_out()
{
    return "2||";
}
std::string add_task(const std::string& task, const std::string& date, const std::string& time, const std::string& username, int priority)
{
    return "3||" + task + "||" + date + "||" + time + "||" + username + "||" + std::to_string(priority) + "\0";
}

std::string get_history(const std::string& username) {
    return "4||" + username + "\0";
}

std::string get_all_tasks(const std::string& username) {
    return "5||" + username + "\0";
}

std::string get_waiting(const std::string& username) {
    return "6||" + username + "\0";
}
