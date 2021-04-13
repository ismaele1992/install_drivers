#include "NetworkAdapters.h"

NetworkAdapters::NetworkAdapters()
{
	this->path_driver = "";
	this->model = "";
	this->model_id = "";
}

int NetworkAdapters::GetNumberNetworkAdapters()
{
	return this->network_adapters.size();
}

void NetworkAdapters::ShowListNetworkAdapters()
{
	Logger::getInstance(this->classname)->logging_info() << "Showing detected network adapters : ";
	for (auto it = this->network_adapters.begin(); it != this->network_adapters.end(); ++it) {
		Logger::getInstance(this->classname)->logging_info() << *it;
	}
}

void NetworkAdapters::ShowNetworkAdaptersInformation()
{
	Logger::getInstance(this->classname)->logging_info() << "Model of network adapter: " << this->model;
	Logger::getInstance(this->classname)->logging_info() << "Model ID: " << this->model_id;
	Logger::getInstance(this->classname)->logging_info() << "Path of the driver: " << this->path_driver;
	Logger::getInstance(this->classname)->logging_info() << "Number of network adapters: " << this->GetNumberNetworkAdapters();
	ShowListNetworkAdapters();
}

std::vector<std::string> NetworkAdapters::GetNetworkwHardwareId()
{
	char buffer[256];
	FILE* pipe = _popen(this->wmi_query_network.c_str(), "r");
	if (!pipe) throw std::runtime_error("Failed to open CMD");
	while (fgets(buffer, sizeof buffer, pipe) != NULL) {
		if (std::find(this->network_adapters.begin(), this->network_adapters.end(), buffer) == this->network_adapters.end() && strcmp(buffer, "\r\n")) {
			std::string aux(buffer);
			aux.erase(std::remove(aux.begin(), aux.end(), '"'), aux.end());
			size_t pos = aux.find("\\");
			while (pos != std::string::npos) {
				aux.replace(pos, strlen("\\"), "\\\\");
				pos = aux.find("\\", pos + strlen("\\\\"));
			}
			this->network_adapters.push_back(aux);
		}
	}
	return this->network_adapters;
}

std::string NetworkAdapters::GetNetworkAdapterDrivers()
{
	Json::Value paths;
	std::ifstream drivers_json("drivers.json");
	drivers_json >> paths;
	Logger::getInstance(this->classname)->logging_debug() << paths;
	for (auto it = paths["drivers"]["ethernet"].begin(); it != paths["drivers"]["ethernet"].end(); ++it) {
		Logger::getInstance(this->classname)->logging_debug() << *it;
	}
	return this->path_driver;
}

NetworkAdapters::~NetworkAdapters()
{
}
