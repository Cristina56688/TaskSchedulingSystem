#include "header.h"
#include <iomanip>

#define WTFILE "waitingtasklist.xml"
#define HISTORY_FILE "history.xml"

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
    std::string buff, aux, data_str, ora_str, task_str;

    getline(ss, buff, '|'); 
    getline(ss, task_str, '|');
    getline(ss, data_str, '|');
    getline(ss, ora_str, '|');
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

    std::stringstream ssTask(task_str);
    while (ssTask >> aux) {
        _task.push_back(aux);
    }

    
    std::stringstream ss2(data_str); 
    try {
        if (getline(ss2, aux, '.')) _data.setZi(std::stoi(aux));
        if (getline(ss2, aux, '.')) _data.setLuna(std::stoi(aux));
        if (getline(ss2, aux, '.')) _data.setAn(std::stoi(aux));
    } catch(...) {}

   
    std::stringstream ss3(ora_str); 
    try {
        if (getline(ss3, aux, ':')) _ora.setOra(std::stoi(aux));
        if (getline(ss3, aux, ':')) _ora.setMin(std::stoi(aux));
        if (getline(ss3, aux, ':')) _ora.setSec(std::stoi(aux));
    } catch(...) {}

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
        message+=std::to_string(it->getId()); 
        
        message+="||";
        std::vector<std::string> task=it->getTask();
        std::string t_str = "";
        for(auto i=task.begin(); i!=task.end(); i++)
            t_str += *i+" ";
        if (!t_str.empty()) t_str.pop_back();
        message += t_str;

        message+="||";
        Data data=it->getData();
        std::ostringstream ossD;
        ossD << std::setfill('0') << std::setw(2) << data.getZi() << "."
             << std::setfill('0') << std::setw(2) << data.getLuna() << "."
             << data.getAn();
        message += ossD.str();

        message+="||";
        Ora ora=it->getOra();
        std::ostringstream ossT;
        ossT << std::setfill('0') << std::setw(2) << ora.getOra() << ":"
             << std::setfill('0') << std::setw(2) << ora.getMin() << ":"
             << std::setfill('0') << std::setw(2) << ora.getSec();
        message += ossT.str();

        message+="||" + it->getUserName();
        message+="||WAITING";
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

bool modify_task(const Task& task)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(WTFILE);
    if (err != XML_SUCCESS) {
        return false;
    }

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) return false;

    XMLElement* taskToModify = nullptr;

    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) 
    {
        int currentId = atoi(it->Attribute("id"));
        if (currentId == task.getId()) {
            taskToModify = it;
            break;
        }
    }

    if (taskToModify) {
        std::string task_str="";
        std::vector<std::string> aux=task.getTask();
        for(auto it=aux.begin(); it!=aux.end(); it++)
        {
            task_str+=*it+ " ";
        }

        taskToModify->SetAttribute("task", task_str.c_str());
        taskToModify->SetAttribute("zi", std::to_string(task.getData().getZi()).c_str());
        taskToModify->SetAttribute("luna", std::to_string(task.getData().getLuna()).c_str());
        taskToModify->SetAttribute("an", std::to_string(task.getData().getAn()).c_str());
        taskToModify->SetAttribute("ora", std::to_string(task.getOra().getOra()).c_str());
        taskToModify->SetAttribute("minute", std::to_string(task.getOra().getMin()).c_str());
        taskToModify->SetAttribute("secunde", std::to_string(task.getOra().getSec()).c_str());
        taskToModify->SetAttribute("priority", std::to_string(task.getPriority()).c_str());

        if (doc.SaveFile(WTFILE) == XML_SUCCESS)
            return true;
    }

    return false;
}



std::string create_message_history(const std::string username)
{
    XMLDocument doc;
    XMLError err = doc.LoadFile(HISTORY_FILE);
    if (err != XML_SUCCESS) {
        return "4||0"; 
    }

    auto* root = doc.FirstChildElement("History");
    if (!root) return "4||0";

    std::vector<std::string> historyRecords;
    for (auto* it = root->FirstChildElement("Entry"); it; it = it->NextSiblingElement("Entry")) {
        const char* uAttr = it->Attribute("User");
        std::string user = uAttr ? uAttr : "";
       
        if (user == username) {
            const char* idAttr = it->Attribute("ID");
            const char* cmdAttr = it->Attribute("Command");
            const char* codeAttr = it->Attribute("ExitCode");
            const char* timeAttr = it->Attribute("Time");
            const char* durAttr = it->Attribute("DurationMs");
            
            std::string id = idAttr ? idAttr : "";
            std::string cmd = cmdAttr ? cmdAttr : "";
            std::string code = codeAttr ? codeAttr : "0";
            std::string timeFull = timeAttr ? timeAttr : "";
            std::string duration = durAttr ? durAttr : "0";
            
            
            std::string datePart = timeFull;
            std::string timePart = "";
            size_t spacePos = timeFull.find(' ');
            if (spacePos != std::string::npos) {
                datePart = timeFull.substr(0, spacePos);
                timePart = timeFull.substr(spacePos + 1);
            }

            std::string result = (code == "0") ? "SUCCESS (Code 0)" : "FAILURE (Code " + code + ")";
            
            // Protocol: ID (F1) || Cmd (F2) || Date (F3) || Time (F4) || Result (F5) || Duration (F6)
            historyRecords.push_back(id + "||" + cmd + "||" + datePart + "||" + timePart + "||" + result + "||" + duration + "ms");
        }
    }

    std::string message = "4||" + std::to_string(historyRecords.size());
    for (const auto& record : historyRecords) {
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
                const char* uAttr = it->Attribute("user");
                std::string user = uAttr ? uAttr : "";
                if (user == username) {
                    const char* idAttr = it->Attribute("id");
                    const char* taskAttr = it->Attribute("task");
                    const char* zi = it->Attribute("zi");
                    const char* luna = it->Attribute("luna");
                    const char* an = it->Attribute("an");
                    const char* ora = it->Attribute("ora");
                    const char* minute = it->Attribute("minute");
                    const char* secunde = it->Attribute("secunde");
                    const char* priorityAttr = it->Attribute("priority");
                    
                    std::string id = idAttr ? idAttr : "";
                    std::string task = taskAttr ? taskAttr : "";
                    
                    std::ostringstream ossD;
                    ossD << std::setfill('0') << std::setw(2) << (zi?atoi(zi):0) << "."
                         << std::setfill('0') << std::setw(2) << (luna?atoi(luna):0) << "."
                         << (an?an:"0");
                    std::string d = ossD.str();

                    std::ostringstream ossT;
                    ossT << std::setfill('0') << std::setw(2) << (ora?atoi(ora):0) << ":"
                         << std::setfill('0') << std::setw(2) << (minute?atoi(minute):0) << ":"
                         << std::setfill('0') << std::setw(2) << (secunde?atoi(secunde):0);
                    std::string t = ossT.str();

                    std::string p = priorityAttr ? priorityAttr : "1";
                    
                    // Protocol: ID || Label || Date || Time || User || Status
                    allRecords.push_back(id + "||Priority " + p + ": " + task + "||" + d + "||" + t + "||" + user + "||WAITING");
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
                    const char* idAttr = it->Attribute("ID");
                    const char* cmdAttr = it->Attribute("Command");
                    const char* timeAttr = it->Attribute("Time");
                    
                    std::string id = idAttr ? idAttr : "";
                    std::string cmd = cmdAttr ? cmdAttr : "";
                    std::string timeFull = timeAttr ? timeAttr : "";
                    
                    std::string datePart = timeFull;
                    std::string timePart = "";
                    size_t spacePos = timeFull.find(' ');
                    if (spacePos != std::string::npos) {
                        datePart = timeFull.substr(0, spacePos);
                        timePart = timeFull.substr(spacePos + 1);
                    }
                    
                    // Protocol: ID || Cmd || Date || Time || User || Status
                    allRecords.push_back(id + "||" + cmd + "||" + datePart + "||" + timePart + "||" + user + "||EXECUTED");
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
    XMLError err = doc.LoadFile(WTFILE);
    if (err != XML_SUCCESS) return "6||0";

    auto* root = doc.FirstChildElement(WTFILE);
    if (!root) return "6||0";

    std::vector<std::string> records;
    for (auto* it = root->FirstChildElement("item"); it; it = it->NextSiblingElement("item")) {
        const char* uAttr = it->Attribute("user");
        std::string user = uAttr ? uAttr : "";
        if (user == username) {
            const char* idAttr = it->Attribute("id");
            const char* taskAttr = it->Attribute("task");
            const char* zi = it->Attribute("zi");
            const char* luna = it->Attribute("luna");
            const char* an = it->Attribute("an");
            const char* ora = it->Attribute("ora");
            const char* minute = it->Attribute("minute");
            const char* secunde = it->Attribute("secunde");
            const char* prio = it->Attribute("priority");
            
            std::string id = idAttr ? idAttr : "";
            std::string task = taskAttr ? taskAttr : "";
            
            std::ostringstream ossD;
            ossD << std::setfill('0') << std::setw(2) << (zi?atoi(zi):0) << "."
                 << std::setfill('0') << std::setw(2) << (luna?atoi(luna):0) << "."
                 << (an?an:"0");
            std::string d = ossD.str();

            std::ostringstream ossT;
            ossT << std::setfill('0') << std::setw(2) << (ora?atoi(ora):0) << ":"
                 << std::setfill('0') << std::setw(2) << (minute?atoi(minute):0) << ":"
                 << std::setfill('0') << std::setw(2) << (secunde?atoi(secunde):0);
            std::string tim = ossT.str();

            std::string p = prio ? prio : "1";
            
            // Protocol: ID || Task || Date || Time || User || Priority
            records.push_back(id + "||" + task + "||" + d + "||" + tim + "||" + user + "||" + p);
        }
    }

    std::string message = "6||" + std::to_string(records.size());
    for(const auto& r : records) {
        message += "||" + r;
    }
    return message;
}