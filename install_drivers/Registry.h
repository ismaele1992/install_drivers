#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <istream>
#include "Logger.h"
typedef enum {
	HARDDRIVE,
	MOTHERBOARD,
	NETWORK
} DRIVER;
class Registry
{
private:
	const char* classname = "Registry";
	const PCWSTR runOnceKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce";
	PCWSTR pathExe;
public:
	Registry();
	Registry(const char* pathExe);
	bool EditRunOnce(DRIVER d);
	~Registry();
};

