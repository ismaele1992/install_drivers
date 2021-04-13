#pragma once
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <istream>
#include <json/value.h>
#include <json/json.h>
#include "Logger.h"
class NetworkAdapters
{
private:
	std::string wmi_query_network = "wmic path win32_pnpentity where \"pnpclass like '%NET%' and (deviceid like '%PCI%' or deviceid like '%USB%') \" get hardwareid";
	std::string model;
	std::string model_id;
	std::vector<std::string> network_adapters;
	std::string path_driver;
	const char* classname = "NetworkAdapters";
public:
	NetworkAdapters();
	int GetNumberNetworkAdapters();
	void ShowListNetworkAdapters();
	void ShowNetworkAdaptersInformation();
	std::vector<std::string> GetNetworkwHardwareId();
	std::string GetNetworkAdapterDrivers();
	~NetworkAdapters();
};

