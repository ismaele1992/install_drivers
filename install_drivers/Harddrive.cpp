#include "Harddrive.h"

Harddrive::Harddrive()
{
	//this->hardisk_size_bytes = GetPartitionsInformation();
	this->GetHardDrives();
	this->GetPartitionsInformation();
}

void Harddrive::GetHardDrives()
{
	char buffer[256];
	FILE* pipe = _popen(this->query_harddrive, "r");
	std::regex condition("([0-9]{1,})([ |\t]*)([0-9]{1,})(.*)");
	std::cmatch cm;
	if (!pipe) throw std::runtime_error("Not able to get harddrives information");
	while (fgets(buffer, sizeof buffer, pipe) != NULL) {
		if (std::regex_search(buffer, cm, condition)) {
			std::map<std::string, std::string> aux;
			aux["Index"] = cm[1];
			aux["Size"] = cm[3];
			aux["SizeGB"] = std::to_string(this->ToGigaBytes(cm[3]));
			this->harddrives.push_back(aux);
		}
	}
}

std::map<std::string, std::string> Harddrive::SelectDiskByIndex(std::string index)
{
	std::map<std::string, std::string> aux;
	int i = 0;
	bool found = false;
	while (i < this->harddrives.size() && !found) {
		if (std::strcmp(this->harddrives[i]["Index"].c_str(), index.c_str()) == 0) {
			aux = this->harddrives[i];
			found = true;
		}
		i++;
	}
	return aux;
}

int Harddrive::GetPartitionsInformation()
{
	char buffer[256];
	FILE* pipe = _popen(this->query_size_unit, "r");
	if (!pipe) throw std::runtime_error("Not able to get size of hardisk from CMD");
	int line = 0;
	std::regex base_regex("([0-9]{1,})([ |\t]*)(.*[:])([ |\t]*)([0-9]{1,})([ |\t]*)([0-9]{1,})(.*)");
	std::cmatch cm;
	while (fgets(buffer, sizeof buffer, pipe) != NULL) {
		if (std::regex_search(buffer, cm, base_regex)) {
			Logger::getInstance(classname)->logging_debug() << "Found partition which matches the search.";
			StorePartition(cm);
		}
	}
	this->ShowPartitions();
	return 0;
}

void Harddrive::StorePartition(std::cmatch cm)
{
	std::map <std::string, std::string> aux;
	aux["Index"] = cm[1];
	aux["Unit"] = cm[3];
	aux["Free_Space"] = cm[5];
	aux["Size"] = cm[7];
	aux["SizeGB"] = std::to_string(this->ToGigaBytes(cm[7]));
	this->hardisk_size_free_bytes = std::stod(aux["Free_Space"]);
	this->hardisk_size_free_gigabytes = this->ToGigaBytes(cm[5]);
	this->hardisk_size_bytes = std::stod(aux["Size"]);
	this->hardisk_size_gigabytes = this->ToGigaBytes(cm[7]);
	this->partitions.push_back(aux);
}

void Harddrive::ShowPartitions()
{
	Logger::getInstance(classname)->logging_info() << "Showing hard drives: ";
	if (this->partitions.size() > 0) {
		for (int i = 0; i < partitions.size(); i++) {
			Logger::getInstance(classname)->logging_debug() << "Index of the hard drive : {0}" << this->partitions[i]["Index"];
			Logger::getInstance(classname)->logging_debug() << "Unit letter of the hard drive : " << this->partitions[i]["Unit"];
			Logger::getInstance(classname)->logging_debug() << "Showing size of the hard drive in Bytes : " << this->partitions[i]["Size"];
			Logger::getInstance(classname)->logging_debug() << "Showing size of the hard drive in Gigabytes : " << this->partitions[i]["SizeGB"];
		}
	}
}

double Harddrive::ToMegaBytes(std::string size)
{
	double msize = std::stod(size);
	for (int i = 0; i < 2; i++) {
		msize = msize / 1024;
	}
	return msize;
}

double Harddrive::ToMegaBytes(double size)
{
	for (int i = 0; i < 2; i++) {
		size = size / 1024;
	}
	return size;
}

double Harddrive::ToGigaBytes(std::string size)
{
	double gsize = std::stod(size);
	for (int i = 0; i < 3; i++) {
		gsize = gsize / 1024;
	}
	return gsize;
}

double Harddrive::ToBytes(std::string size)
{
	double bsize = std::stod(size);
	for (int i = 0; i < 3; i++) {
		bsize = bsize * 1024;
	}
	return bsize;
}

void Harddrive::CopyToDPartition()
{
	namespace fs = std::filesystem;
	const char* source_path = "C:\\Users\\xpad\\Documents\\D\\";
	const char* destination_path = "D:\\";
	Logger::getInstance(classname)->logging_info() << "Copying files to D location";
	if (fs::exists(source_path) && fs::exists(destination_path)) {
		for (const auto& entry : fs::directory_iterator(source_path)) {
			Logger::getInstance(classname)->logging_debug() << entry.path() << std::endl;
			fs::copy(entry.path(), destination_path);
		}
	}
	else {
		Logger::getInstance(classname)->logging_error() << "There is at least a location which is not valid.";
	}
}

void Harddrive::SetUpPartitions()
{
	// Seach for C unit
	int i = 0;
	bool enc = false;
	int disk_units = 0;
	int j = 0;
	const char* diskpart_file = "diskpart.txt";
	while (i < this->partitions.size() && disk_units <= 1){
		if (std::strcmp("0", this->partitions[i]["Index"].c_str()) == 0) {
			Logger::getInstance(this->classname)->logging_debug() << "Found disk with index 0 and letter " << this->partitions[i]["Unit"];
			disk_units++;
			j = i;
		}
		i++;
	}
	if (disk_units == 1) {
		double free_space = stod(this->partitions[j]["Free_Space"]);
		// Calculating used space by the partition
		double occupied_space = std::stod(this->partitions[j]["Size"]) - free_space;
		Logger::getInstance(this->classname)->logging_debug() << "Used space in bytes : " << occupied_space;
		Logger::getInstance(this->classname)->logging_debug() << "Used space in Gigabytes : " << this->ToGigaBytes(std::to_string(occupied_space));
		std::map<std::string, std::string> aux = this->SelectDiskByIndex(this->partitions[j]["Index"]);
		// Check if the query of the physical drive returns something
		if (aux.size() > 0) {
			double space_to_extend = (std::stod(aux["Size"]) / 2) - this->ToBytes("20") - occupied_space;
			Logger::getInstance(this->classname)->logging_info() << "Available space to extend in bytes : " << space_to_extend;
			Logger::getInstance(this->classname)->logging_info() << "Available space to extend in Gigabytes : " << this->ToGigaBytes(std::to_string(space_to_extend));
			if (std::stod(aux["SizeGB"]) > 400) {
				this->CreateDiskPartScript(space_to_extend, this->partitions[j]["Index"], diskpart_file);
				this->RunDiskPart(diskpart_file);
			}
			else {
				Logger::getInstance(this->classname)->logging_error() << "Not enough space to do partitioning.";
			}
		}
	}
	else {
		Logger::getInstance(this->classname)->logging_warning() << "This disk has already more than one partition created. Skipping process of creating partitions";
	}
}

void Harddrive::CreateDiskPartScript(double size, std::string disk, const char* diskpart_file)
{
	std::ofstream of(diskpart_file, std::ios::out);
	if (of) {
		of << "select disk " << disk << std::endl;
		of << "select partition 2" << std::endl;
		of << "extend size=" << std::to_string(std::llround(this->ToMegaBytes(size))) << std::endl;
		of << "select disk " << disk << std::endl;
		of << "create partition primary" << std::endl;
		of << "assign letter=D" << std::endl;
		of << "format quick label=\"Extended\" fs=ntfs" << std::endl;
		of << "EXIT" << std::endl;
		of.close();
	}
}

void Harddrive::RunDiskPart(const char* file)
{
	std::string command = "diskpart /s ";
	char buffer[256];
	command.append(file);
	FILE* pipe = _popen(command.c_str(), "r");
	if (!pipe) throw std::runtime_error("Not able to run diskpart on CMD");
	while (fgets(buffer, sizeof buffer, pipe) != NULL) {
		Logger::getInstance(classname)->logging_debug() << buffer << std::endl;
	}
	_pclose(pipe);
}

Harddrive::~Harddrive()
{
}
