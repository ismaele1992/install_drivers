#include "Motherboard.h"

Motherboard::Motherboard()
{
	this->model = "";
	this->driver_path = "";
}

void Motherboard::ShowMotherboardInformation()
{
	std::cout << "----------------------------------------------------------" << std::endl;
	std::cout << "Showing information of the Motherboard:"					  << std::endl;
	std::cout << "Model of the motherboard : "				   << this->model << std::endl;
	std::cout << "Path to drivers of the motherboard : " << this->driver_path << std::endl;
	std::cout << "----------------------------------------------------------" << std::endl;
}

std::string Motherboard::GetMotherboardInformation()
{
	char buffer[256];
	std::fstream of("output.txt", std::ios::out);
	FILE* pipe = _popen(this->query_motherboard.c_str(), "r");
	if (!pipe) throw std::runtime_error("Failed to run CMD command.");
	if (of.is_open()) {
		try {
			while (fgets(buffer, sizeof buffer, pipe) != NULL) {
				std::cout << strcmp(buffer, "\n") << '\n';
				std::string out = buffer;
				out.erase(std::remove_if(out.begin(), out.end(), ::isspace), out.end());
				if (strcmp(out.c_str(), "") != 0) {
					of << out << '\n';
					this->model = out;
				}
			}
		}
		catch (...) {
			_pclose(pipe);
			of.close();
			throw;
		}
	}
	else {
		std::cout << "Cannot create output.txt file" << '\n';
	}
	of.close();
	_pclose(pipe);
	std::cout << this->model << '\n';
	return this->model;
}

std::string Motherboard::GetDriverMotherboard(char* drivers_file)
{
	Json::Value drivers;
	std::ifstream drivers_json(drivers_file);
	drivers_json >> drivers;
	std::cout << drivers["drivers"] << std::endl;
	std::string model_motherboard = GetMotherboardInformation();
	try {
		int i = 0;
		bool found = false;
		while (i < drivers["drivers"]["motherboard"].size() && !found) {
			std::string model = drivers["drivers"]["motherboard"][i]["name"].asString();
			if (strcmp(model_motherboard.c_str(), model.c_str()) == 0) {
				std::cout << "Found driver on JSON file" << std::endl;
				std::cout << "Path to the driver location of the model " << model << " is : " << drivers["drivers"]["motherboard"][i]["path"];
				this->driver_path = drivers["drivers"]["motherboard"][i]["path"].asString();
				found = true;
			}
			i++;
		}
	}
	catch (...) {
		std::cout << "There is no driver available for the provided motherboard.";
	}
	return this->driver_path;
}

Motherboard::~Motherboard()
{
	this->driver_path = "";
	this->model = "";
	this->query_motherboard = "";
}
