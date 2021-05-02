#include <string>
#include <Windows.h>
#include <iostream>
#include <shlobj.h>
#include <SFML/Window/Keyboard.hpp>
#include "file_handlers.h"
#include "../gameData.h"
#include "convert.h"

void fileHandler::saveKeybinds() {
	// get all the require data
	auto ins = Data::getInstance();
	auto keyBinds = ins->getKeybinds();
	auto path = ins->getFilePath();
	auto folder = ins->getFileFolder();

	// get the sea chain folder location (roaming/.seachain)
	auto seachainpath = path.append(folder);

	std::string output;

	// loop through the keybinds adding the keys to the string
	for (auto it = keyBinds->begin(); it != keyBinds->end(); it++) {
		output += Converter::SFKeyToString(it->second);
		output += "\n";
	}

	// if the seachain folders don't exist create them
	if (!std::filesystem::exists(seachainpath))
		std::filesystem::create_directories(seachainpath);


	std::ofstream controls;
	// write to the keybinds file in the path with all the controls
	controls.open(seachainpath.append("controls.txt"));
	controls << output << std::endl;
	controls.close();
}

void fileHandler::loadKeybinds() {
	// get the required data
	auto ins = Data::getInstance();
	auto path = ins->getFilePath();
	auto folder = ins->getFileFolder();
	auto keyBinds = ins->getKeybinds();

	// make a vector of all the keybinds
	int index = 0;
	std::vector<std::string> keyControls;

	// loop through the keybinds map adding all the possible keybinds to the vector.
	for (auto it = keyBinds->begin(); it != keyBinds->end(); it++) {
		keyControls.push_back(it->first);
	}

	// get the controls file in the sea chain path
	auto seaChainPath = path.append(folder);
	std::string key;

	std::ifstream controls(seaChainPath.append("controls.txt"));

	// read the file line by line
	while (std::getline(controls, key)) {
		if (key == "")
			break;

		ins->setKeybind(keyControls[index], Converter::StringToSFKey(key));
		index++;
	}
}

void fileHandler::saveSettings() {
	// get all the require data
	auto ins = Data::getInstance();
	auto path = ins->getFilePath();
	auto folder = ins->getFileFolder();

	// get the sea chain folder location (roaming/.seachain)
	auto seachainpath = path.append(folder);

	std::string output;

	// loop through the keybinds adding the keys to the string
	output += std::to_string(ins->getMusicVolume()) += "\n";
	output += std::to_string(ins->getSoundVolume()) += "\n";
	output += std::to_string(Engine::getVsync()) += "\n";
	output += std::to_string(Engine::getFramerate()) += "\n";

	// if the seachain folders don't exist create them
	if (!std::filesystem::exists(seachainpath))
		std::filesystem::create_directories(seachainpath);


	std::ofstream controls;
	// write to the keybinds file in the path with all the controls
	controls.open(seachainpath.append("settings.txt"));
	controls << output << std::endl;
	controls.close();
}

void fileHandler::loadSettings() {
	// get the required data
	auto ins = Data::getInstance();
	auto path = ins->getFilePath();
	auto folder = ins->getFileFolder();

	// make a vector of all the keybinds
	std::vector<std::string> settings;

	// get the controls file in the sea chain path
	auto seaChainPath = path.append(folder);
	std::string setting;

	std::ifstream controls(seaChainPath.append("settings.txt"));

	// read the file line by line
	while (std::getline(controls, setting)) {
		if (setting == "")
			break;

		settings.push_back(setting);
	}

	ins->setMusicVolume(stof(settings[0]));
	ins->setSoundVolume(stof(settings[1]));
	Engine::setVsync(stringToBool(settings[2]));
	Engine::setFramerate(stoi(settings[3]));
}

bool fileHandler::stringToBool(std::string s){
	if (s == "1")
		return true;
	else
		return false;
}