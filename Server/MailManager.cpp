#include "header.h"

void sendAccountCreationConfirmation(const std::string mail)
{
	if (mail.empty()) return;
	std::string subject = "Creare cont TASK SCHEDULING SYSTEM";
	std::string body = "Contul dumneavoastra a fost creat cu succes.";
	std::string command = "python3 send_mail.py \"" + mail + "\" \"" + subject + "\" \"" + body + "\" \"" SENDER_EMAIL "\" \"" SENDER_PASSWORD "\"";
	
	std::cout << "[MAIL] Executing: " << command << std::endl;
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "[MAIL] Eroare la trimiterea mail-ului de confirmare catre " << mail << " (res=" << res << ")\n";
	} else {
		std::cout << "[MAIL] Comanda send_mail.py a fost executata cu succes pentru " << mail << "\n";
	}
}

void sendTaskFailureNotification(const std::string mail, int taskId, int exitCode, const std::string& taskCommand)
{
	if (mail.empty()) return;
	std::string subject = "Eroare Task " + std::to_string(taskId);
	std::string body = "Task-ul cu ID " + std::to_string(taskId) + " (\\\"" + taskCommand + "\\\") s-a incheiat cu codul de eroare " + std::to_string(exitCode) + ".";
	std::string command = "python3 send_mail.py \"" + mail + "\" \"" + subject + "\" \"" + body + "\" \"" SENDER_EMAIL "\" \"" SENDER_PASSWORD "\"";

	std::cout << "[MAIL] Executing: " << command << std::endl;
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "[MAIL] Eroare la trimiterea mail-ului de eroare catre " << mail << " (res=" << res << ")\n";
	} else {
		std::cout << "[MAIL] Comanda send_mail.py a fost executata cu succes pentru " << mail << "\n";
	}
}

void sendAbnormalDurationNotification(const std::string mail, int taskId, long long duration_ms, const std::string& taskCommand)
{
	if (mail.empty()) return;
	std::string subject = "Alerta Durata Task " + std::to_string(taskId);
	std::string body = "Task-ul cu ID " + std::to_string(taskId) + " (\\\"" + taskCommand + "\\\") a avut o durata de executie anormala de " + std::to_string(duration_ms) + " ms.";
	std::string command = "python3 send_mail.py \"" + mail + "\" \"" + subject + "\" \"" + body + "\" \"" SENDER_EMAIL "\" \"" SENDER_PASSWORD "\"";

	std::cout << "[MAIL] Executing: " << command << std::endl;
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "[MAIL] Eroare la trimiterea mail-ului de alerta catre " << mail << " (res=" << res << ")\n";
	} else {
		std::cout << "[MAIL] Comanda send_mail.py a fost executata cu succes pentru " << mail << "\n";
	}
}