#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <istream>
typedef enum {
	HARDDRIVE,
	MOTHERBOARD,
	NETWORK
} DRIVER;
class Registry
{
private:
	std::string run_once_reg_key = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce";
	std::string run_once_reg_key_user = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce";
	std::string installer_path = "C:\\Users\\Isma_VM\\Documents\\install_drivers\\Debug\\install_drivers.exe";
public:
	Registry();
	bool EditRunOnce(DRIVER d);
	~Registry();
};

