#pragma once
#include <memory>
#include <string>
#include "engine.h"
#include <filesystem>
#include <fstream>
#include <SFML/Audio/Music.hpp>
#include <map>
#include <SFML/Window/Keyboard.hpp>

class Converter
{

public:
	static std::string SFKeyToString(sf::Keyboard::Key key);
	static sf::Keyboard::Key UInt32ToSFKey(sf::Uint32 key);
};