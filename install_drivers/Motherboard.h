#pragma once
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <istream>
#include <json/value.h>
#include <json/json.h>
class Motherboard
{
private:
	std::string query_motherboard = "wmic baseboard get product";
	std::string model;
	std::string driver_path;
public:
	Motherboard();
	void ShowMotherboardInformation();
	std::string GetMotherboardInformation();
	std::string GetDriverMotherboard(char * drivers_file);
	~Motherboard();
};

