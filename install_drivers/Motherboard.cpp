#include "Motherboard.h"

Motherboard::Motherboard()
{
	this->model = "";
	this->driver_path = "";
}

void Motherboard::ShowMotherboardInformation()
{
	Logger::getInstance(this->classname)->logging_debug() << "Showing information of the Motherboard.";
	Logger::getInstance(this->classname)->logging_info() << "Model of the motherboard : " << this->model;
	Logger::getInstance(this->classname)->logging_info() << "Path to drivers of the motherboard : " << this->driver_path;
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
				Logger::getInstance(this->classname)->logging_debug() << strcmp(buffer, "\n");
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
		Logger::getInstance(this->classname)->logging_error() << "Cannot create output.txt file";
	}
	of.close();
	_pclose(pipe);
	Logger::getInstance(this->classname)->logging_debug() << this->model;
	return this->model;
}

std::string Motherboard::GetDriverMotherboard(char* drivers_file)
{
	Json::Value drivers;
	std::ifstream drivers_json(drivers_file);
	drivers_json >> drivers;
	Logger::getInstance(this->classname)->logging_debug() << drivers["drivers"];
	std::string model_motherboard = GetMotherboardInformation();
	try {
		int i = 0;
		bool found = false;
		while (i < drivers["drivers"]["motherboard"].size() && !found) {
			std::string model = drivers["drivers"]["motherboard"][i]["name"].asString();
			if (strcmp(model_motherboard.c_str(), model.c_str()) == 0) {
				Logger::getInstance(this->classname)->logging_debug() << "Found driver on JSON file";
				Logger::getInstance(this->classname)->logging_debug() << "Path to the driver location of the model " << model << " is : " << drivers["drivers"]["motherboard"][i]["path"];
				this->driver_path = drivers["drivers"]["motherboard"][i]["path"].asString();
				found = true;
			}
			i++;
		}
	}
	catch (...) {
		Logger::getInstance(this->classname)->logging_warning() << "There is no driver available for the provided motherboard.";
	}
	return this->driver_path;
}

Motherboard::~Motherboard()
{
	this->driver_path = "";
	this->model = "";
	this->query_motherboard = "";
}
