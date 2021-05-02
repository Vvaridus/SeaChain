#pragma once
#include <memory>
#include <string>
#include "engine.h"
#include <filesystem>
#include <fstream>
#include <SFML/Audio/Music.hpp>
#include <map>
#include <SFML/Window/Keyboard.hpp>

class fileHandler
{

public:
	static void saveKeybinds();
	static void loadKeybinds();

	static void saveSettings();
	static void loadSettings();

	static bool stringToBool(std::string s);
};