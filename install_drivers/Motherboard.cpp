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

std::string Motherboard::GetDriverMotherboard(const char* drivers_file)
{
	Json::Value drivers;
	std::ifstream drivers_json(drivers_file);
	drivers_json >> drivers;
	Logger::getInstance(this->classname)->logging_debug() << drivers["drivers"];
	try {
		int i = 0;
		bool found = false;
		while (i < drivers["drivers"]["motherboard"].size() && !found) {
			std::string model = drivers["drivers"]["motherboard"][i]["name"].asString();
			if (strcmp(this->model.c_str(), model.c_str()) == 0) {
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

void Motherboard::SetUpModel(const char* model)
{
	this->model = model;
}

void Motherboard::SetUpDriverPath(const char* driver_path)
{
	this->driver_path = driver_path;
}

bool Motherboard::UnzipDriver(const char* source_path, const char* destination_path)
{
	namespace fs = std::filesystem;
	Logger::getInstance(this->classname)->logging_debug() << "Unzipping File : " << source_path << " into : " << destination_path;
	int err = 0;
	zip* zip_pfile = zip_open(source_path, 0, &err);
	for (int i = 0; i < zip_get_num_entries(zip_pfile, 0); i++) {
		std::cout << i << std::endl;
		const char* file_name = zip_get_name(zip_pfile, i, 0);
		struct zip_stat st;
		zip_stat_init(&st);
		zip_stat(zip_pfile, file_name, 0, &st);
		char* contents = new char[st.size];
		zip_file* p = zip_fopen(zip_pfile, file_name, 0);
		zip_fread(p, contents, st.size);
		std::fstream out_file;
		char path_extracted_file[1024];	// Preparing full path to store unzipped files
		strcpy_s(path_extracted_file, destination_path);
		strcat_s(path_extracted_file, "\\");
		strcat_s(path_extracted_file, file_name);
		std::cout << path_extracted_file << std::endl;
		std::replace(std::begin(path_extracted_file), std::end(path_extracted_file), '/', '\\');
		char * last_pos = std::strrchr(path_extracted_file, '\\');
		char* new_sp = path_extracted_file;
		char old = new_sp[last_pos - path_extracted_file];
		new_sp[last_pos-path_extracted_file] = '\0';
		Logger::getInstance(this->classname)->logging_debug() << "Creating the location : " << new_sp;
		CreateDirectory(new_sp, NULL);
		new_sp[last_pos - path_extracted_file] = old;
		out_file.open(path_extracted_file, 'w');
		if (out_file) {
			out_file.write(contents, st.size);
		}
		Logger::getInstance(this->classname)->logging_debug() << "Extracted file : " << path_extracted_file;
		out_file.close();
		zip_fclose(p);
		delete[] contents;
	}
	zip_close(zip_pfile);
	return false;
}

void Motherboard::InstallDriver()
{
	std::cout << "Install driver section." << std::endl;
	Logger::getInstance(this->classname)->logging_debug() << this->driver_path;
	for (auto & a : std::filesystem::directory_iterator(this->driver_path)) {
		Logger::getInstance(this->classname)->logging_debug() << a.path();
		Logger::getInstance(this->classname)->logging_debug() << "Parent path: " << a.path().parent_path().string().c_str();
		Logger::getInstance(this->classname)->logging_debug() << "Extension of the file : " << a.path().extension();
		if (!a.is_directory() && a.path().extension() == ".zip") {
			Logger::getInstance(this->classname)->logging_debug() << "Found Zip file.";
			UnzipDriver(a.path().string().c_str(), a.path().parent_path().string().c_str());
			//std::string b(a.path().c_str());
			//UnzipDriver(a.path().c_str(), this->driver_path.c_str());
			//a.file_size();
		}
	}
	/*FILE* pipe = _popen(driver_path.c_str(), "r");
	if (!pipe) {
		Logger::getInstance(this->classname)->logging_error() << "Not able to open CMD.";
		throw std::runtime_error("Not able to open CMD.");
	}
	char buffer[256];
	Logger::getInstance(this->classname)->logging_info() << "Installing driver for the motherboard with model " << this->model;
	while (fgets(buffer, sizeof buffer, pipe) != NULL) {
		Logger::getInstance(this->classname)->logging_debug() << buffer;
	}
	Logger::getInstance(this->classname)->logging_info() << "Installation is completed.";*/
}

Motherboard::~Motherboard()
{
	this->driver_path = "";
	this->model = "";
	this->query_motherboard = "";
}
