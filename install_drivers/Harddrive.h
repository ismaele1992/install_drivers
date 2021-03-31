#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <fstream>
#include <cmath>
class Harddrive
{
private:
	double hardisk_size_bytes;
	double hardisk_size_gigabytes;
	double hardisk_size_free_bytes;
	double hardisk_size_free_gigabytes;
	const char* query_size = "wmic logicaldisk get size";
	const char* query_size_unit = "wmic logicaldisk where \"Description = 'Local Fixed Disk'\" get access,caption,size,freespace";
	const char* query_harddrive = "wmic diskdrive where \"deviceid like '%PHYSICAL%'\" get index,size";
	std::vector<std::map<std::string, std::string>> partitions;
	std::vector<std::map<std::string, std::string>> harddrives;
public:
	Harddrive();
	void GetHardDrives();
	int GetPartitionsInformation();
	void StorePartition(std::cmatch cm);
	void ShowPartitions();
	double ToMegaBytes(std::string size);
	double ToMegaBytes(double size);
	double ToGigaBytes(std::string size);
	double ToBytes(std::string size);
	void SetUpPartitions();
	void CreateDiskPartScript(double size, std::string disk, const char* diskpart_file);
	void RunDiskPart(const char* file);
	~Harddrive();
};

