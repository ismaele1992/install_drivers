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
	std::cout << "Showing detected network adapters : " << std::endl;
	for (auto it = this->network_adapters.begin(); it != this->network_adapters.end(); ++it) {
		std::cout << *it << std::endl;
	}
}

void NetworkAdapters::ShowNetworkAdaptersInformation()
{
	std::cout << this->model << std::endl;
	std::cout << this->model_id << std::endl;
	std::cout << this->path_driver << std::endl;
	std::cout << this->GetNumberNetworkAdapters() << std::endl;
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
	std::cout << paths << std::endl;
	for (auto it = paths["drivers"]["ethernet"].begin(); it != paths["drivers"]["ethernet"].end(); ++it) {
		std::cout << *it << std::endl;
	}
	return this->path_driver;
}

NetworkAdapters::~NetworkAdapters()
{
}
