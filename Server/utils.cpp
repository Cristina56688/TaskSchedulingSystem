#include "header.h"

int getIndex(const std::string& str) {
    std::istringstream ss(str);
    std::string index;
    std::getline(ss, index, '|');
    return std::stoi(index);
}

std::vector<std::string> split(const std::string& s) {

    std::vector<std::string> out;

    std::string temp = s;
    size_t pos = 0;
    while ((pos = temp.find("||", pos)) != std::string::npos) {
        temp.replace(pos, 2, "|");
        pos += 1;
    }

    std::string cur;
    std::istringstream ss(temp);
    while (std::getline(ss, cur, '|')) out.push_back(cur);
    return out;
}


