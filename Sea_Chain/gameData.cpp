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
int Data::soundVolume;
sf::Music Data::music;
std::unordered_map<std::string, sf::Keyboard::Key> Data::keybinds {
	{"MOVE_UP", sf::Keyboard::W},
	{"MOVE_LEFT", sf::Keyboard::A},
	{"MOVE_RIGHT", sf::Keyboard::D},
	{"MOVE_DOWN", sf::Keyboard::S},
	{"INTERACT", sf::Keyboard::E},
	{"GO_BACK", sf::Keyboard::Tab}
};

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

void Data::setMusicFile(std::string path) {
	music.openFromFile(path);
}

void Data::setMusicLoop(bool loop) {
	music.setLoop(loop);
}

void Data::playMusic(bool play)
{
	if (play)
		music.play();
	else if (!play)
		music.stop();
}

void Data::setMusicVolume(float volume) {
	music.setVolume((volume * 10.f));
}

float Data::getMusicVolume() {
	return (music.getVolume() / 10);
}

void Data::setSoundVolume(float volume) {
	soundVolume = (volume * 10.f);
}

float Data::getSoundVolume() {
	return (soundVolume / 10);
}

std::shared_ptr<std::unordered_map<std::string, sf::Keyboard::Key>> Data::getKeybinds()
{
	return std::make_shared<std::unordered_map<std::string, sf::Keyboard::Key>>(keybinds);
}

void Data::setKeybind(std::string keybind, sf::Keyboard::Key key) {
	keybinds.find(keybind)->second = key;
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
	soundVolume = 50.f;
	music.setVolume(50.f);
}
