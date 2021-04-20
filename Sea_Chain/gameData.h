#pragma once
#include <memory>
#include <string>
#include "engine.h"
#include <filesystem>
#include <fstream>

class Data
{
private:
	static std::shared_ptr<Data> instance;
	static std::shared_ptr<Entity> player;
	static std::filesystem::path filePath;
	static std::string fileFolder;
	static std::string currentLogFile;

public:
	Data();
	static std::shared_ptr<Data> getInstance();
	static std::shared_ptr<Entity> getPlayer();
	static std::filesystem::path getFilePath();
	static std::string getFileFolder();
	static std::string getLogFile();
	static void setLogFile(std::string logFile);
	static void setPlayer(std::shared_ptr<Entity> p);
};