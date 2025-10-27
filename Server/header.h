#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <tinyxml2.h>
#include <iostream>
#include <regex>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

class MacPass
{
private:
    std::string _mac;
    std::string _password;
public:
    MacPass():_mac(""), _password(""){};
    MacPass(std::string str);
    std::string getMac() {return _mac;}
    std::string getPassword() {return _password;}
};

static bool isValidMac(const std::string& mac);
bool macExists(std::vector<std::pair<std::string, std::string>> existing, std::string mac);
void addMacPassword(const std::string& filePath, const std::string& mac, const std::string& password);

#endif