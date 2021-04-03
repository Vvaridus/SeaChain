#include "logger.h"
#include <iostream>
#include <chrono>
#include <string>

std::string Logger::getTime() {
	time_t t;
	struct tm* tt;
	time(&t);
	tt = gmtime(&t);
	std::string currentTime = asctime(tt);
	currentTime.pop_back();

	return currentTime;
}

void Logger::addEvent(const EventType type, const Action action, const std::string message)
{
	std::string stringType, stringAction;

	// should use a map or something else 
	// instead of the two enum class and two switches

	switch (type) {
		case EventType::Engine: stringType = "Engine"; break;
		case EventType::Tile_Loader: stringType = "Tile Loader"; break;
		case EventType::Tag: stringType = "Tag"; break;
		case EventType::Scene: stringType = "Scene"; break;
		case EventType::Entity: stringType = "Entity"; break;
	}

	switch (action) {
		case Action::Loaded: stringAction = "Loaded"; break;
		case Action::Unloaded: stringAction = "Unloaded"; break;
		case Action::Loading: stringAction = "Loading"; break;
		case Action::Updated: stringAction = "Updated"; break;
		case Action::Scene_Change: stringAction = "Scene_Change"; break;
		case Action::Entering_Loading_Screen: stringAction = "Entering_Loading_Screen"; break;
		case Action::Leaving_Loading_Screen: stringAction = "Leaving_Loading_Screen"; break;
		case Action::Interaction: stringAction = "Interaction"; break;
		case Action::Entity_Created: stringAction = "Entity_Created"; break;
		case Action::Entity_Deleted: stringAction = "Entity_Deleted"; break;
	}

	std::cout << "[" << getTime() << "]" << "[" << stringType << "]" << "[" << stringAction << "]" << " " << message << std::endl;
}

