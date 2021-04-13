#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <ctime>
#include <string>
constexpr auto MAX_SIZE = 256;
class Logger
{
private:
	char log_folder_path[MAX_SIZE];
	char log_file_path[MAX_SIZE];
	char classname[80];
	std::ofstream log_file;
	static Logger* logger;
protected:
	std::ostringstream os;
public:
	Logger();
	Logger(const char* log_path);
	static Logger * getInstance(const char * _classname);
	void setClassName(const char* classname);
	std::ofstream& logging_warning();
	std::ofstream& logging_error();
	std::ofstream& logging_debug();
	std::ofstream& logging_info();
	virtual ~Logger();
};
