#pragma once
#include <memory>
#include <string>
#include "engine.h"
#include <filesystem>
#include <fstream>
#include <SFML/Audio/Music.hpp>
#include <map>
#include <SFML/Window/Keyboard.hpp>

class Data
{
private:
	static std::shared_ptr<Data> instance;
	static std::shared_ptr<Entity> player;
	static std::filesystem::path filePath;
	static std::string fileFolder;
	static std::string currentLogFile;
	static bool debugMode;
	static int soundVolume;
	static int musicVolume;
	static sf::Music music;
	static std::unordered_map<std::string, sf::Keyboard::Key> keybinds;

public:
	Data();
	static std::shared_ptr<Data> getInstance();
	static std::shared_ptr<Entity> getPlayer();
	static std::filesystem::path getFilePath();
	static std::string getFileFolder();
	static std::string getLogFile();
	static void setLogFile(std::string logFile);
	static void setPlayer(std::shared_ptr<Entity> p);
	static bool getDebug();
	static void setMusicFile(std::string path);
	static void setMusicLoop(bool loop);
	static void playMusic(bool play); // High lights as function definition not found
	static void setSoundVolume(float volume); // High lights as function definition not found
	static float getSoundVolume();
	static void setMusicVolume(float volume);
	static float getMusicVolume();
	static std::shared_ptr<std::unordered_map<std::string, sf::Keyboard::Key>> getKeybinds();
};