#pragma once
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <istream>
#include <json/value.h>
#include <json/json.h>
#include "Logger.h"
#include <filesystem>
#include <zip.h>
class Motherboard
{
private:
	const char* classname = "Motherboard";
	std::string query_motherboard = "wmic baseboard get product";
	std::string model;
	std::string driver_path;
public:
	Motherboard();
	void ShowMotherboardInformation();
	std::string GetMotherboardInformation();
	std::string GetDriverMotherboard(const char * drivers_file);
	void SetUpModel(const char * model);
	void SetUpDriverPath(const char * driver_path);
	bool UnzipDriver(const char * source_path, const char * destination_path);
	void InstallDriver();
	~Motherboard();
};

