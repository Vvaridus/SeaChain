#include "logger.h"
#include <fstream>
#include <chrono>
#include <string>
#include "../Sea_Chain/gameData.h"

std::string Logger::getTime() {
	time_t t;
	struct tm* tt;
	time(&t);
	tt = localtime(&t); // get the current date and time in UTC format.

	// Easily take the time into an appropriate format. Keeps the logs readable.
	std::string currentTime = std::to_string(tt->tm_hour) + ":" + std::to_string(tt->tm_min) + ":" + std::to_string(tt->tm_sec);
	return currentTime;
}

std::string Logger::getDate() {
	time_t t;
	struct tm* tt;
	time(&t);
	tt = localtime(&t); // get the current date and time in UTC format.

	// Easily take the time into an appropriate format. Keeps the logs readable.
	//std::string currentDate = std::to_string(tt->tm_mday) + std::to_string(tt->tm_mon) + std::to_string(tt->tm_year) + std::to_string(tt->tm_hour) + std::to_string(tt->tm_min) + std::to_string(tt->tm_sec);
	std::string currentDate = std::to_string(tt->tm_mday) + "." + std::to_string(tt->tm_mon + 1) + "." + std::to_string(tt->tm_year + 1900) + " ~ " + std::to_string(tt->tm_hour) + "-" + std::to_string(tt->tm_min) + "-" + std::to_string(tt->tm_sec);
	return currentDate;
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
		case EventType::Logging: stringType = "Logging"; break;
	}

	switch (action) {
		case Action::Loaded: stringAction = "Loaded"; break;
		case Action::Unloaded: stringAction = "Unloaded"; break;
		case Action::Loading: stringAction = "Loading"; break;
		case Action::Updated: stringAction = "Updated"; break;
		case Action::Scene_Change: stringAction = "Scene Change"; break;
		case Action::Entering_Loading_Screen: stringAction = "Entering Loading Screen"; break;
		case Action::Leaving_Loading_Screen: stringAction = "Leaving Loading Screen"; break;
		case Action::Interaction: stringAction = "Interaction"; break;
		case Action::Entity_Created: stringAction = "Entity reated"; break;
		case Action::Entity_Deleted: stringAction = "Entity Deleted"; break;
		case Action::Item_Added: stringAction = "Item Added"; break;
		case Action::Item_Removed: stringAction = "Item Removed"; break;
		case Action::Item_Changed: stringAction = "Item Changed"; break;
		case Action::CreatingFolder: stringAction = "Item Changed"; break;
		case Action::CreatingLogFile: stringAction = "Item Changed"; break;
	}

	// Output the time, EventType and action to the console.
	// Eventually add log files.
	std::string logMessage = "[" + getTime() + "]" + "[" + stringType + "]" + "[" + stringAction + "]" + " " + message;
	std::cout << logMessage << std::endl;
	fileOutput(logMessage);
}

void Logger::fileOutput(std::string log) {
	auto ins = Data::getInstance(); 
	auto path = ins->getFilePath();
	auto folder = ins->getFileFolder();
	// add the file and folder path together
	auto completepath = path.append(folder); 

	// check if the logs folder exists, if not create it.
	if (!std::filesystem::exists(completepath.append("logs")))
		std::filesystem::create_directories(completepath.append("logs"));

	// check if their is an exisitng log file for this session, if not create a new session. 
	if (ins->getLogFile() == "") {
		ins->setLogFile(getDate() + ".txt");
	}

	std::ofstream file;
	// write to the path and log folder with the title.
	file.open(completepath.append(ins->getLogFile()), std::ios::app);
	file << log << std::endl;
	file.close();
}

