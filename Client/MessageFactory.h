#ifndef MESSAGE_FACTORY_H
#define MESSAGE_FACTORY_H

#include <string>

class MessageFactory {
public:
    static std::string mesaj_autentificare(const std::string& mac, const std::string& password);
};

#endif
