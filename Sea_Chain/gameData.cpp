#include "gameData.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <shlobj.h>


std::shared_ptr<Data> Data::instance;
std::shared_ptr<Entity> Data::player;
std::filesystem::path Data::filePath;
std::string Data::fileFolder;

std::shared_ptr<Data> Data::getInstance() {
	if (instance == nullptr) {
		instance = std::make_shared<Data>();
		PWSTR path_tmp;
		auto folder_path = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path_tmp);
		filePath = path_tmp;
		CoTaskMemFree(path_tmp);
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

void Data::setPlayer(std::shared_ptr<Entity> p) {
	player = std::move(p);
}

Data::Data() {}
