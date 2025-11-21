#include "header.h"

void sendAccountCreationConfirmation(const std::string mail)
{
	std::string message = "echo 'Contul dumneavoastră a fost creat cu succes.' | mail -s 'Creare cont' ";
	message += mail;
	system(message.c_str());
	
}