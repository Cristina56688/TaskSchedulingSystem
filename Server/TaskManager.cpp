#include "header.h"

#define WTFILE "waitingtasklist.xml"

bool exists(const int& x, const std::vector<int>& list)
{
    for(auto it=list.begin(); it!=list.end(); it++)
        if(x==*it)
            return true;

    return false;
}

int generate_id(std::vector<int> existing)
{
    std::random_device rd;                        
    std::mt19937 gen(rd());                        
    std::uniform_int_distribution<> distr(1, INT_MAX); 

    int randomNumber = distr(gen);
    while(exists(randomNumber, existing))
        randomNumber=distr(gen);
        
    return randomNumber;
}

Task::Task(std::string str):_id(-1)
{
    std::string temp = str;
    size_t pos = 0;
    while ((pos = temp.find("||", pos)) != std::string::npos) {
        temp.replace(pos, 2, "|");
        pos += 1;
    }

    std::stringstream ss(temp);
    std::string buff, aux, data, ora, userName;

    getline(ss, buff, '|');
    getline(ss, buff, '|');
    getline(ss, data, '|');
    getline(ss, ora, '|');
    getline(ss, _user, '|');

      std::string priorityStr;
    if (getline(ss, priorityStr, '|')) {
        try {
            _priority = std::stoi(priorityStr);
        } catch (...) {
            _priority = 1; 
        }
    } else {
        _priority = 1; 
    }

    std::stringstream ss2(data); 
    getline(ss2, aux, '.');
    _data.setZi(std::stoi(aux));
    getline(ss2, aux, '.');
    _data.setLuna(std::stoi(aux));
    getline(ss2, aux, '.');
    _data.setAn(std::stoi(aux));

    std::stringstream ss3(ora); 
    getline(ss3, aux, ':');
    _ora.setOra(std::stoi(aux));
    getline(ss3, aux, ':');
    _ora.setMin(std::stoi(aux));
    getline(ss3, aux, ':');
    _ora.setSec(std::stoi(aux));

    std::stringstream ss4(buff);
    while(ss4>>aux)
        _task.push_back(aux);

}

Task::Task(const int id, const std::string task, const Data data, const Ora ora, const std::string user, const int priority):_data(data), _ora(ora), _user(user), _id(id), _priority(priority)
{
    _task.clear();

    std::stringstream ss(task);
    std::string aux;

    while(ss>>aux)
        _task.push_back(aux);
}


bool save_task(const std::string& filePath, const Task& task)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(filePath.c_str());
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement(WTFILE);
        doc.InsertEndChild(root);
    }
    else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) {
        root = doc.NewElement(WTFILE);
        doc.InsertEndChild(root);
    }


    std::vector<int> existing;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) 
    {
        int id = atoi(it->Attribute("id"));
        if (id) existing.emplace_back(id);
    }

    int id=generate_id(existing);

    std::string task_str="";
    std::vector<std::string> aux=task.getTask();
    for(auto it=aux.begin(); it!=aux.end(); it++)
    {
        task_str+=*it+ " ";
    }

    XMLElement* item = doc.NewElement("item");
    item->SetAttribute("id", std::to_string(id).c_str());
    item->SetAttribute("user", task.getUserName().c_str());
    item->SetAttribute("task", task_str.c_str());
    item->SetAttribute("zi", std::to_string(task.getData().getZi()).c_str());
    item->SetAttribute("luna", std::to_string(task.getData().getLuna()).c_str());
    item->SetAttribute("an", std::to_string(task.getData().getAn()).c_str());
    item->SetAttribute("ora", std::to_string(task.getOra().getOra()).c_str());
    item->SetAttribute("minute", std::to_string(task.getOra().getMin()).c_str());
    item->SetAttribute("secunde", std::to_string(task.getOra().getSec()).c_str());
    item->SetAttribute("priority", std::to_string(task.getPriority()).c_str());
   
    root->InsertEndChild(item);

    if (doc.SaveFile(filePath.c_str()) != XML_SUCCESS)
        throw std::runtime_error("Eroare la salvarea XML");

    return true;
}


std::vector<Task> extract_waiting_tasks()
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(WTFILE);
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement(WTFILE);
        doc.InsertEndChild(root);
    }
    else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) {
        root = doc.NewElement(WTFILE);
        doc.InsertEndChild(root);
    }


    std::vector<Task> existing;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) 
    {
        int id = atoi(it->Attribute("id"));
        std::string user=it->Attribute("user");
        std::string task=it->Attribute("task");
        int zi = atoi(it->Attribute("zi"));
        int luna = atoi(it->Attribute("luna"));
        int an = atoi(it->Attribute("an"));
        int ora = atoi(it->Attribute("ora"));
        int minute = atoi(it->Attribute("minute"));
        int secunde = atoi(it->Attribute("secunde"));

        existing.push_back(Task(id, task, Data(zi, luna, an), Ora(ora, minute, secunde), user));
    }

    return existing;
}

std::string create_message_login(const std::string username)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(WTFILE);
    if (err == XML_ERROR_FILE_NOT_FOUND) {
        auto* decl = doc.NewDeclaration(R"(xml version="1.0" encoding="UTF-8")");
        doc.InsertFirstChild(decl);
        auto* root = doc.NewElement(WTFILE);
        doc.InsertEndChild(root);
    }
    else if (err != XML_SUCCESS) {
        throw std::runtime_error("Eroare la deschiderea XML");
    }

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) {
        root = doc.NewElement(WTFILE);
        doc.InsertEndChild(root);
    }


    std::vector<Task> tasks;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) 
    {
        std::string user=it->Attribute("user");
        if(user==username)
       { 
        int id = atoi(it->Attribute("id"));
        std::string task=it->Attribute("task");
        int zi = atoi(it->Attribute("zi"));
        int luna = atoi(it->Attribute("luna"));
        int an = atoi(it->Attribute("an"));
        int ora = atoi(it->Attribute("ora"));
        int minute = atoi(it->Attribute("minute"));
        int secunde = atoi(it->Attribute("secunde"));

        tasks.push_back(Task(id, task, Data(zi, luna, an), Ora(ora, minute, secunde), user));
    }

    }

    std::string message="2||"+std::to_string(tasks.size());
    for(auto it=tasks.begin(); it!=tasks.end(); it++)
    {
        message+="||";
        std::vector<std::string> task=it->getTask();
        for(auto i=task.begin(); i!=task.end(); i++)
            message+=*i+" ";
        message.pop_back();

        message+="||";
        Data data=it->getData();
        message+=std::to_string(data.getZi())+"."+std::to_string(data.getLuna())+"."+std::to_string(data.getAn());

        message+="||";
        Ora ora=it->getOra();
        message+=std::to_string(ora.getOra())+":"+std::to_string(ora.getMin())+":"+std::to_string(ora.getSec());
        
    }

    return message;
}

bool remove_task(const int id)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(WTFILE);
    if (err != XML_SUCCESS) {
        return false;
    }

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) return false;

    XMLElement* taskToDelete = nullptr;

    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) 
    {
        int currentId = atoi(it->Attribute("id"));
        if (currentId == id) {
            taskToDelete = it;
            break;
        }
    }

    if (taskToDelete) {
        root->DeleteChild(taskToDelete);
        if (doc.SaveFile(WTFILE) == XML_SUCCESS)
            return true;
    }

    return false;
}


std::string create_message_history(const std::string username)
{
    XMLDocument doc;
#define HISTORY_FILE "history.xml"
    XMLError err = doc.LoadFile(HISTORY_FILE);
    if (err != XML_SUCCESS) {
        return "4||0"; 
    }

    auto* root = doc.FirstChildElement("History");
    if (!root) return "4||0";

    std::vector<std::string> historyRecords;
    for (auto* it = root->FirstChildElement("Entry"); it; it = it->NextSiblingElement("Entry")) 
    {
        const char* uAttr = it->Attribute("User");
        std::string user = uAttr ? uAttr : "";
       
        if (user == username) {
            std::string id = it->Attribute("ID");
            std::string cmd = it->Attribute("Command");
            std::string code = it->Attribute("ExitCode");
            std::string time = it->Attribute("Time");
            
            historyRecords.push_back(id + "||" + cmd + "||" + code + "||" + time);
        }
    }

    std::string message = "4||" + std::to_string(historyRecords.size());
    for(const auto& record : historyRecords) {
        message += "||" + record;
    }
    return message;
}



std::string create_message_all(const std::string username) {
    XMLDocument docWait;
    XMLError errWait = docWait.LoadFile(WTFILE);
    std::vector<std::string> allRecords;
    
    if (errWait == XML_SUCCESS) {
        auto* root = docWait.FirstChildElement(WTFILE);
        if (root) {
             for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
                std::string user = it->Attribute("user");
                if (user == username) {
                    std::string id = it->Attribute("id");
                    std::string task = it->Attribute("task");
                    std::string d = it->Attribute("zi"); d += "."; d += it->Attribute("luna"); d += "."; d += it->Attribute("an");
                    std::string t = it->Attribute("ora"); t += ":"; t += it->Attribute("minute"); t += ":"; t += it->Attribute("secunde");
                    
                    
                    allRecords.push_back(id + "||" + task + "||" + d + " " + t + "||WAITING");
                }
             }
        }
    }

    XMLDocument docHist;
    XMLError errHist = docHist.LoadFile(HISTORY_FILE);
    if (errHist == XML_SUCCESS) {
        auto* root = docHist.FirstChildElement("History");
        if (root) {
             for (auto* it = root->FirstChildElement("Entry"); it; it = it->NextSiblingElement("Entry")) {
                const char* uAttr = it->Attribute("User");
                std::string user = uAttr ? uAttr : "";
                if (user == username) {
                    std::string id = it->Attribute("ID");
                    std::string cmd = it->Attribute("Command");
                    std::string code = it->Attribute("ExitCode");
                    std::string time = it->Attribute("Time");
                    
                    std::string status = "FINISHED (Code " + code + ")";
                    allRecords.push_back(id + "||" + cmd + "||" + time + "||" + status);
                }
             }
        }
    }

    std::string message = "5||" + std::to_string(allRecords.size());
    for(const auto& record : allRecords) {
        message += "||" + record;
    }
    return message;
}



std::string create_message_waiting(const std::string username) {
    XMLDocument doc;
#define WTFILE "waitingtasklist.xml"
    XMLError err = doc.LoadFile(WTFILE);
    if (err != XML_SUCCESS) return "6||0";

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) return "6||0";

    std::vector<std::string> records;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        std::string user = it->Attribute("user");
        if (user == username) {
            std::string id = it->Attribute("id");
            std::string task = it->Attribute("task");
            std::string zi = it->Attribute("zi");
            std::string luna = it->Attribute("luna");
            std::string an = it->Attribute("an");
            std::string ora = it->Attribute("ora");
            std::string minute = it->Attribute("minute");
            std::string secunde = it->Attribute("secunde");
            
            std::string d = zi + "." + luna + "." + an;
            std::string tim = ora + ":" + minute + ":" + secunde;
            
            records.push_back(id + "||" + task + "||" + d + "||" + tim);
        }
    }

    std::string message = "6||" + std::to_string(records.size());
    for(const auto& r : records) {
        message += "||" + r;
    }
    return message;
}