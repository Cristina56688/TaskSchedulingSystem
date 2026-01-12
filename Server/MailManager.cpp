#include "header.h"

void sendAccountCreationConfirmation(const std::string mail)
{
	if (mail.empty()) return;
	std::string subject = "Account Creation TASK SCHEDULING SYSTEM";
	std::string body = "Your account has been successfully created.";
	std::string command = "python3 send_mail.py \"" + mail + "\" \"" + subject + "\" \"" + body + "\" \"" SENDER_EMAIL "\" \"" SENDER_PASSWORD "\"";
	
	std::cout << "[MAIL] Executing: " << command << std::endl;
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "[MAIL] Error sending confirmation email to " << mail << " (res=" << res << ")\n";
	} else {
		std::cout << "[MAIL] send_mail.py command executed successfully for " << mail << "\n";
	}
}

void sendTaskFailureNotification(const std::string mail, int taskId, int exitCode, const std::string& taskCommand)
{
	if (mail.empty()) return;
	std::string subject = "Task Error " + std::to_string(taskId);
	std::string body = "Task with ID " + std::to_string(taskId) + " (\\\"" + taskCommand + "\\\") finished with error code " + std::to_string(exitCode) + ".";
	std::string command = "python3 send_mail.py \"" + mail + "\" \"" + subject + "\" \"" + body + "\" \"" SENDER_EMAIL "\" \"" SENDER_PASSWORD "\"";

	std::cout << "[MAIL] Executing: " << command << std::endl;
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "[MAIL] Error sending error notification email to " << mail << " (res=" << res << ")\n";
	} else {
		std::cout << "[MAIL] send_mail.py command executed successfully for " << mail << "\n";
	}
}

void sendAbnormalDurationNotification(const std::string mail, int taskId, long long duration_ms, const std::string& taskCommand)
{
	if (mail.empty()) return;
	std::string subject = "Task Duration Alert " + std::to_string(taskId);
	std::string body = "Task with ID " + std::to_string(taskId) + " (\\\"" + taskCommand + "\\\") had an abnormal execution duration of " + std::to_string(duration_ms) + " ms.";
	std::string command = "python3 send_mail.py \"" + mail + "\" \"" + subject + "\" \"" + body + "\" \"" SENDER_EMAIL "\" \"" SENDER_PASSWORD "\"";

	std::cout << "[MAIL] Executing: " << command << std::endl;
	int res = system(command.c_str());
	if (res != 0) {
		std::cerr << "[MAIL] Error sending alert email to " << mail << " (res=" << res << ")\n";
	} else {
		std::cout << "[MAIL] send_mail.py command executed successfully for " << mail << "\n";
	}
}