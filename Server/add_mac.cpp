#include <tinyxml2.h>
#include <iostream>
#include <regex>
#include <vector>
#include <algorithm>
#include <sstream>
#include "header.h"

using namespace tinyxml2;

#define FILENAME "macpasswordlist.xml"


MacPass::MacPass(std::string str)
{
    std::istringstream ss(str);
    std::string mac, password;

    std::getline(ss, mac, '|');
    std::getline(ss, mac, '|');
    std::getline(ss, password, '|');

    _mac=mac;
    _password=password;
}

std::string noramlization_mac(std::string mac)
{
    std::string nm(mac);
    for(long unsigned int i=0; i<mac.size(); i++)
            nm[i]=std::tolower(mac[i]);

    return mac;
}

static bool isValidMac(const std::string& mac) {
    static const std::regex re("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");
    return std::regex_match(mac, re);
}

bool macExists(std::vector<std::pair<std::string, std::string>> existing, std::string mac)
{
    for(auto it=existing.begin(); it!=existing.end(); it++)
    {
        if(it->first==mac)
            return true;
    }

    return false;
}

void addMacPassword(const std::string& filePath, const std::string& mac, const std::string& password)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(filePath.c_str());
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement(FILENAME);
        doc.InsertEndChild(root);
    } else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement(FILENAME);
    if (!root) {
        root = doc.NewElement(FILENAME);
        doc.InsertEndChild(root);
    }

    std::string nm=noramlization_mac(mac);

    // Construim o lista (id, mac) existenta pentru a evita duplicatele

    std::vector<std::pair<std::string, std::string>> existing;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        const char* m = it->Attribute("mac");
        const char* p = it->Attribute("password");

         if (m && p) {
            std::string macExist(m);
            std::string passExist(p);
            existing.emplace_back(macExist, passExist);
        }
    }




    if (!isValidMac(nm)) 
        {
        std::cerr << "MAC invalid ignorat: " << mac << "\n";
            return;
        }

    if(!macExists(existing, nm))
    {

        XMLElement* item = doc.NewElement("item");
        item->SetAttribute("mac", nm.c_str());
        item->SetAttribute("password", password.c_str());
        root->InsertEndChild(item);

        // Tinem minte ca sa evitam duplicatele în acelasi apel
        existing.emplace_back(nm, password);
    

        if (doc.SaveFile(filePath.c_str()) != XML_SUCCESS) 
            throw std::runtime_error("Eroare la salvarea XML");

    }
    else 
    {
        for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) 
        {
            const char* m = it->Attribute("mac");

            if(m==nm)
            {
                it->SetAttribute("password", password.c_str());

                if (doc.SaveFile(filePath.c_str()) != XML_SUCCESS) 
                    throw std::runtime_error("Eroare la salvarea XML");

                break;
            
            }
        }
    }
}

