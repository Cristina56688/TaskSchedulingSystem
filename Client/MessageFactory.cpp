#include "MessageFactory.h"


std::string MessageFactory::mesaj_autentificare(const std::string& mac, const std::string& password) {
    return "0|" + mac + "|" + password;
}
