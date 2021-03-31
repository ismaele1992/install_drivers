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
		if (line == 1) {
			if (std::regex_search(buffer, cm, base_regex)) {
				std::cout << "Found condition " << std::endl;
				StorePartition(cm);
			}
		}
		line++;
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
	std::cout << "Showing hard drives: " << std::endl;
	if (this->partitions.size() > 0) {
		for (int i = 0; i < partitions.size(); i++) {
			std::cout << "Index of the hard drive : " << this->partitions[i]["Index"] << std::endl;
			std::cout << "Unit letter of the hard drive : " << this->partitions[i]["Unit"] << std::endl;
			std::cout << "Showing size of the hard drive in Bytes : " << this->partitions[i]["Size"] << std::endl;
			std::cout << "Showing size of the hard drive in Gigabytes : " << this->partitions[i]["SizeGB"] << std::endl;
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
			std::cout << "Found disk with index 0 and letter " << this->partitions[i]["Unit"] << std::endl;
			disk_units++;
			j = i;
		}
		i++;
	}
	if (disk_units == 1) {
		double free_space = stod(this->partitions[j]["Free_Space"]);
		// Calculating used space by the partition
		double occupied_space = std::stod(this->partitions[j]["Size"]) - free_space;
		std::cout << "Used space in bytes : " << occupied_space << std::endl;
		std::cout << "Used space in Gigabytes : " << this->ToGigaBytes(std::to_string(occupied_space)) << std::endl;
		std::map<std::string, std::string> aux = this->SelectDiskByIndex(this->partitions[j]["Index"]);
		// Check if the query of the physical drive returns something
		if (aux.size() > 0) {
			double space_to_extend = (std::stod(aux["Size"]) / 2) - this->ToBytes("20") - occupied_space;
			std::cout << "Available space to extend in bytes : " << space_to_extend << std::endl;
			std::cout << "Available space to extend in Gigabytes : " << this->ToGigaBytes(std::to_string(space_to_extend)) << std::endl;
			if (std::stod(aux["SizeGB"]) > 400) {
				this->CreateDiskPartScript(space_to_extend, this->partitions[j]["Index"], diskpart_file);
				this->RunDiskPart(diskpart_file);
			}
			else {
				std::cout << "Not enough space to do partitioning." << std::endl;
			}
		}
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
		std::cout << buffer << std::endl;
	}
	_pclose(pipe);
}

Harddrive::~Harddrive()
{
}
