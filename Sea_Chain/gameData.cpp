#include "gameData.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <shlobj.h>

// Store the singleton instance
std::shared_ptr<Data> Data::instance;
// store a player entity
std::shared_ptr<Entity> Data::player;
// store the file path to %appdata%
std::filesystem::path Data::filePath;
// Store the default folder name for the application inside the file path
std::string Data::fileFolder;
// the current log file of the session
std::string Data::currentLogFile = "";
bool Data::debugMode;

// method to get the current instance or create one
std::shared_ptr<Data> Data::getInstance() {
	if (instance == nullptr) {
		instance = std::make_shared<Data>();		
	}

	return instance;
}

std::shared_ptr<Entity> Data::getPlayer() {
	return player;
}

std::filesystem::path Data::getFilePath() {
	return filePath;
}
std::string Data::getFileFolder() {
	return fileFolder;
}

std::string Data::getLogFile()
{
	return currentLogFile;
}

void Data::setLogFile(std::string logFile)
{
	currentLogFile = logFile;
}

void Data::setPlayer(std::shared_ptr<Entity> p) {
	player = std::move(p);
}

bool Data::getDebug() {
	return debugMode;
}

Data::Data() {
	// to hold the address of a pointer that SHGetKnownFolderPath returns for the known Folder.
	PWSTR path_tmp; 
	// using Win32 API, get a known folder to the OS. Ignore DWord flag by setting to 0, setting to nullptr means it'll look for %appdata% for the current user.
	// and the last parameter is the address of pointer.
	auto folder_path = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
	// Convert it to the filesystem::path
	filePath = path_tmp;
	CoTaskMemFree(path_tmp); // free up that memory
	
	player = nullptr;
	fileFolder = ".seachain";
	debugMode = true;
}
