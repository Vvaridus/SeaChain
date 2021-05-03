#include "gameData.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <shlobj.h>
#include <logger.h>

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
// to display hitboxes
bool Data::debugMode;
// volume of sounds
int Data::soundVolume;
// music theme
sf::Music Data::musicTheme;
// battle theme
sf::Music Data::musicBattle;
// keybinds and the associated keys
std::unordered_map<std::string, sf::Keyboard::Key> Data::keybinds {
	{"MOVE_UP", sf::Keyboard::W},
	{"MOVE_LEFT", sf::Keyboard::A},
	{"MOVE_RIGHT", sf::Keyboard::D},
	{"MOVE_DOWN", sf::Keyboard::S},
	{"INTERACT", sf::Keyboard::E},
	{"GO_BACK", sf::Keyboard::Tab}
};
bool Data::biscuitMode;

// method to get the current instance or create one
std::shared_ptr<Data> Data::getInstance() {
	if (instance == nullptr) {
		instance = std::make_shared<Data>();		
	}

	return instance;
}

// get the player
std::shared_ptr<Entity> Data::getPlayer() {
	return player;
}

// get the file path (%appdata%)
std::filesystem::path Data::getFilePath() {
	return filePath;
}

// get file folder (.seachain(
std::string Data::getFileFolder() {
	return fileFolder;
}

// get the current log file in use
std::string Data::getLogFile()
{
	return currentLogFile;
}

// set the current log file to use
void Data::setLogFile(std::string logFile)
{
	currentLogFile = logFile;
}

// set the current player to use
void Data::setPlayer(std::shared_ptr<Entity> p) {
	player = std::move(p);
}

bool Data::getDebug() {
	return debugMode;
}

void Data::setDebug(bool d) {
	debugMode = d;
}

// set the music file path to open
void Data::setMusicFile(std::string path) {
	Logger::addEvent(Logger::EventType::Audio, Logger::Action::Loading, "Game Theme");
	musicTheme.openFromFile(path);
	Logger::addEvent(Logger::EventType::Audio, Logger::Action::Loaded, "Game Theme");
}

void Data::setMusicLoop(bool loop) {
	musicTheme.setLoop(loop);
}

void Data::playMusic(bool play)
{
	if (play)
	{
		Logger::addEvent(Logger::EventType::Audio, Logger::Action::Playing, "Game Theme");
		musicTheme.play();
	}
	else if (!play) {
		Logger::addEvent(Logger::EventType::Audio, Logger::Action::Paused, "Game Theme");
		musicTheme.stop();
	}
}

void Data::setMusicVolume(float volume) {
	musicTheme.setVolume((volume * 10.f));
}

float Data::getMusicVolume() {
	return (musicTheme.getVolume() / 10);
}

// set the battle music file to open
void Data::setMusicFileBattle(std::string path) {
	Logger::addEvent(Logger::EventType::Audio, Logger::Action::Loading, "Battle Theme");
	musicBattle.openFromFile(path);
	Logger::addEvent(Logger::EventType::Audio, Logger::Action::Loaded, "Battle Theme");
}

void Data::setMusicLoopBattle(bool loop) {
	musicBattle.setLoop(loop);
}

void Data::playMusicBattle(bool play)
{
	if (play) {
		Logger::addEvent(Logger::EventType::Audio, Logger::Action::Playing, "Battle Theme");
		musicBattle.play();
	}
	else if (!play) {
		Logger::addEvent(Logger::EventType::Audio, Logger::Action::Loading, "Battle Theme");
		musicBattle.stop();
	}
}

void Data::setMusicVolumeBattle(float volume) {
	musicBattle.setVolume((volume * 10.f));
}

float Data::getMusicVolumeBattle() {
	return (musicBattle.getVolume() / 10);
}



void Data::setSoundVolume(float volume) {
	soundVolume = (volume * 10.f);
}

float Data::getSoundVolume() {
	return (soundVolume / 10);
}

// get the current keybinds
std::shared_ptr<std::unordered_map<std::string, sf::Keyboard::Key>> Data::getKeybinds()
{
	return std::make_shared<std::unordered_map<std::string, sf::Keyboard::Key>>(keybinds);
}

// set a keybind
void Data::setKeybind(std::string keybind, sf::Keyboard::Key key) {
	Logger::addEvent(Logger::EventType::Keybind, Logger::Action::Updated, "");
	keybinds.find(keybind)->second = key;
}

void Data::setBiscuit(bool b) {
	biscuitMode = b;
}
bool Data::getBiscuit() {
	return biscuitMode;
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
	
	// set default values
	player = nullptr;
	fileFolder = ".seachain";
	debugMode = false;
	soundVolume = 50.f;
	musicTheme.setVolume(50.f);
	musicBattle.setVolume(20.f);
}
