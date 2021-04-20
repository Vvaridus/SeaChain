#include <string>
#include <map>

class Logger {
public:
	// Enums so that logs are as strongly typed as possible
	// Also makes sure their is no inconsistencies in the logs. 
	// Does add issues to easily adding new ones.
	static enum class EventType {
		Engine,
		Tile_Loader,
		Tag,
		Scene,
		Entity,
		Inventory,
		Logging
	};
	static enum class Action {
		Loaded,
		Unloaded,
		Loading,
		Updated,
		Scene_Change,
		Entering_Loading_Screen,
		Leaving_Loading_Screen,
		Interaction,
		Entity_Created,
		Entity_Deleted,
		Item_Added,
		Item_Removed,
		Item_Changed,
		CreatingFolder,
		CreatingLogFile
	};

	static void addEvent(const Logger::EventType type, const Logger::Action action, const std::string message);
private:
	static void fileOutput(std::string log);
	static std::string getTime();
	static std::string Logger::getDate();
};