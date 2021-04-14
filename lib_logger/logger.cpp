#include "logger.h"
#include <iostream>
#include <chrono>
#include <string>

std::string Logger::getTime() {
	time_t t;
	struct tm* tt;
	time(&t);
	tt = gmtime(&t); // get the current date and time in UTC format.

	// Easily take the time into an appropriate format. Keeps the logs readable.
	std::string currentTime = std::to_string(tt->tm_hour) + ":" + std::to_string(tt->tm_min) + ":" + std::to_string(tt->tm_sec);
	return currentTime;
}

void Logger::addEvent(const EventType type, const Action action, const std::string message)
{
	std::string stringType, stringAction;

	// should use a map or something else 
	// instead of the two enum class and two switches

	// Since enums value names can't be printed, use a switch to store them in a string
	switch (type) {
		case EventType::Engine: stringType = "Engine"; break;
		case EventType::Tile_Loader: stringType = "Tile Loader"; break;
		case EventType::Tag: stringType = "Tag"; break;
		case EventType::Scene: stringType = "Scene"; break;
		case EventType::Entity: stringType = "Entity"; break;
		case EventType::Inventory: stringType = "Inventory"; break;
	}

	switch (action) {
		case Action::Loaded: stringAction = "Loaded"; break;
		case Action::Unloaded: stringAction = "Unloaded"; break;
		case Action::Loading: stringAction = "Loading"; break;
		case Action::Updated: stringAction = "Updated"; break;
		case Action::Scene_Change: stringAction = "Scene_Change"; break;
		case Action::Entering_Loading_Screen: stringAction = "Entering Loading Screen"; break;
		case Action::Leaving_Loading_Screen: stringAction = "Leaving Loading Screen"; break;
		case Action::Interaction: stringAction = "Interaction"; break;
		case Action::Entity_Created: stringAction = "Entity reated"; break;
		case Action::Entity_Deleted: stringAction = "Entity Deleted"; break;
		case Action::Item_Added: stringAction = "Item Added"; break;
		case Action::Item_Removed: stringAction = "Item Removed"; break;
		case Action::Item_Changed: stringAction = "Item Changed"; break;
	}

	// Output the time, EventType and action to the console.
	// Eventually add log files.
	std::cout << "[" << getTime() << "]" << "[" << stringType << "]" << "[" << stringAction << "]" << " " << message << std::endl;
}

