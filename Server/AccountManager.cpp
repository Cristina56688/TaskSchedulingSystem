
#include "header.h"

#define FILENAME "accountslist.xml"


SingInData::SingInData(const std::string str)
{
    std::string temp = str;
    size_t pos = 0;
    while ((pos = temp.find("||", pos)) != std::string::npos) {
        temp.replace(pos, 2, "|");
        pos += 1;
    }

    std::stringstream ss(temp);
    getline(ss, _mail, '|');
    getline(ss, _mail, '|');
    getline(ss, _userName, '|');
    getline(ss, _password, '|');
}

LogInData::LogInData(const std::string str)
{
    std::string temp = str;
    size_t pos = 0;
    while ((pos = temp.find("||", pos)) != std::string::npos) {
        temp.replace(pos, 2, "|");
        pos += 1;
    }

    std::stringstream ss(temp);
    getline(ss, _userName, '|');
    getline(ss, _userName, '|');
    getline(ss, _password, '|');
}

bool accountExists(std::vector<std::pair<std::string, std::string>> existing, std::string userName, std::string password)
{
    for (auto it = existing.begin(); it != existing.end(); it++)
    {
        if (it->first == userName && it->second == password)
            return true;
    }

    return false;
}

bool addAccount(const std::string& filePath, const SingInData& sd)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(filePath.c_str());
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement(FILENAME);
        doc.InsertEndChild(root);
    }
    else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement(FILENAME);
    if (!root) {
        root = doc.NewElement(FILENAME);
        doc.InsertEndChild(root);
    }


    std::vector<std::pair<std::string, std::string>> existing;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        const char* u = it->Attribute("userName");
        const char* p = it->Attribute("password");

        if (u && p) {
            std::string userExist(u);
            std::string passExist(p);
            existing.emplace_back(userExist, passExist);
        }
    }

    if (!accountExists(existing, sd.getUserName(), sd.getPassword()))
    {

        XMLElement* item = doc.NewElement("item");
        item->SetAttribute("userName", sd.getUserName().c_str());
        item->SetAttribute("password", sd.getPassword().c_str());
        item->SetAttribute("mail", sd.getMail().c_str());

        root->InsertEndChild(item);

        // Tinem minte ca sa evitam duplicatele în acelasi apel
        existing.emplace_back(sd.getUserName(), sd.getPassword());


        if (doc.SaveFile(filePath.c_str()) != XML_SUCCESS)
            throw std::runtime_error("Eroare la salvarea XML");

        return true;

    }
    else return false;

    return false;
}

bool verifyAccount(const std::string& filePath, const LogInData& ld)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(filePath.c_str());
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement(FILENAME);
        doc.InsertEndChild(root);
    }
    else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement(FILENAME);
    if (!root) {
        root = doc.NewElement(FILENAME);
        doc.InsertEndChild(root);
    }


    // Construim o lista (username, password) existenta pentru a evita duplicatele

    std::vector<std::pair<std::string, std::string>> existing;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        const char* u = it->Attribute("userName");
        const char* p = it->Attribute("password");

        if (u && p) {
            std::string userExist(u);
            std::string passExist(p);
            existing.emplace_back(userExist, passExist);
        }
    }

    if (accountExists(existing, ld.getUserName(), ld.getPassword()))
        return true;

    return false;
}

std::string getMailByUserName(const std::string& userName)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(FILENAME);
    if (err != XML_SUCCESS) return "";

    auto* root = doc.FirstChildElement(FILENAME);
    if (!root) return "";

    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        const char* u = it->Attribute("userName");
        const char* m = it->Attribute("mail");

        if (u && m && std::string(u) == userName) {
            return std::string(m);
        }
    }

    return "";
}