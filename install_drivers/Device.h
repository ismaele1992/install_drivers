#pragma once
#include <string>
class Device
{
protected:
	const char* classname;
	std::string query_device;
	std::string model;
	std::string driver_path;
public:
	Device();
	void ShowDeviceInformation();
	void GetDeviceInformation();
	void GetDriverDevice();
	void InstallDriver();
	virtual ~Device();
};

