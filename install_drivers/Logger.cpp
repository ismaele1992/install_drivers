#include "Logger.h"

Logger* Logger::logger{ nullptr };

Logger::Logger()
{
	GetCurrentDirectoryA(MAX_SIZE, this->log_folder_path);
	strcat_s(this->log_file_path, this->log_folder_path);
	strcat_s(this->log_file_path, "\\log.txt");
	strcpy_s(this->classname, "Logger");
	this->os.str("");
	this->os.clear();
	try
	{
		this->log_file.open(this->log_file_path, 'w');
	}
	catch (const std::runtime_error)
	{
		throw("Location to create the file is not valid.");
		this->log_file.close();
	}
}

Logger::Logger(const char* log_path)
{
	strcat_s(this->log_folder_path, log_path);
	strcat_s(this->log_file_path, log_path);
	strcpy_s(this->classname, "Logger");
	this->os.str("");
	this->os.clear();
	try
	{
		this->log_file.open(this->log_file_path, 'w');
	}
	catch (const std::runtime_error)
	{
		throw("Location to create the file is not valid.");
		this->log_file.close();
	}
}

Logger* Logger::getInstance(const char* _classname)
{
	if (logger == nullptr) {
		logger = new Logger();
	}
	strcpy_s(logger->classname, _classname);
	std::remove(std::begin(logger->classname), std::end(logger->classname), '\n');
	return logger;
}

std::ofstream& Logger::logging_warning()
{
	time_t _time = time(NULL);
	char buffer[80];
	struct tm timeinfo;
	localtime_s(&timeinfo, &_time);
	std::strftime(buffer, 80, "%H:%M:%S-%d/%m/%Y", &timeinfo);
	std::remove(std::begin(buffer), std::end(buffer), '\n');
	this->log_file << std::endl << buffer << "[" << this->classname << "]" << "[WARNING]:" << '\t';
	return this->log_file;
}

std::ofstream& Logger::logging_error()
{
	time_t _time = time(NULL);
	char buffer[80];
	struct tm timeinfo;
	localtime_s(&timeinfo, &_time);
	std::strftime(buffer, 80, "%H:%M:%S-%d/%m/%Y", &timeinfo);
	std::remove(std::begin(buffer), std::end(buffer), '\n');
	this->log_file << std::endl << buffer << "[" << this->classname << "]" << "[ERROR]:" << '\t';
	return this->log_file;
}

void Logger::setClassName(const char* classname)
{
	strcpy_s(this->classname, classname);
	std::remove(std::begin(this->classname), std::end(this->classname), '\n');
}

std::ofstream& Logger::logging_debug()
{
	time_t _time = time(NULL);
	char buffer[80];
	struct tm timeinfo;
	localtime_s(&timeinfo, &_time);
	std::strftime(buffer, 80, "%H:%M:%S-%d/%m/%Y", &timeinfo);
	std::remove(std::begin(buffer), std::end(buffer), '\n');
	this->log_file << std::endl << buffer << "[" << this->classname << "]" << "[DEBUG]:" << '\t';
	return this->log_file;
}

std::ofstream& Logger::logging_info()
{
	time_t _time = time(NULL);
	char buffer[80];
	struct tm timeinfo;
	localtime_s(&timeinfo, &_time);
	std::strftime(buffer, 80, "%H:%M:%S-%d/%m/%Y", &timeinfo);
	std::remove(std::begin(buffer), std::end(buffer), '\n');
	this->log_file << std::endl << buffer << "[" << this->classname << "]" << "[INFO]:" << '\t';
	return this->log_file;
}

Logger::~Logger()
{
	if (this->log_file) {
		this->log_file.close();
	}
}
