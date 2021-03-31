#include "Registry.h"

Registry::Registry()
{
}

bool Registry::EditRunOnce(DRIVER d)
{
	char buffer[256];
	bool run = false;
	std::string next_device = "";
	switch (d) {
	case HARDDRIVE:
		next_device = "-m";
		break;
	case MOTHERBOARD:
		next_device = "-n";
		break;
	case NETWORK:
		next_device = "";
		break;
	}
	// If next device is empty that means there is no additional runonce entry to add, which means
	// the registry is not editted.
	if (std::strcmp(next_device.c_str(), "") == 0) {
		std::cout << "No need to edit Run Once" << std::endl;
	}
	else {
		std::string command = "reg add ";
		command += this->run_once_reg_key_user;
		command += " /v Install /t REG_SZ /d \"";
		command += this->installer_path;
		command += " ";
		command += next_device;
		command += "\" /f\n";
		std::cout << command << std::endl;
		//system(command.c_str());
		FILE* pipe = _popen(command.c_str(), "r");
		if (!pipe) {
			throw std::runtime_error("Not able to run drivers.exe");
		}
		else {
			run = true;
		}
		Sleep(5000);
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			std::cout << buffer << std::endl;
		}
		_pclose(pipe);
	}
	return run;
}

Registry::~Registry()
{
}
