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
		Logging,
		System,
		FileHandler,
		Audio,
		Keybind,
		Attack
	};
	static enum class Action {
		Loaded,
		Unloaded,
		Loading,
		Updated,
		SceneChange,
		EnteringLoadingScreen,
		LeavingLoadingScreen,
		Interaction,
		EntityCreated,
		EntityDeleted,
		AddedComponent,
		RemovedComponent,
		ItemAdded,
		ItemRemoved,
		ItemChanged,
		BiscuitsChanged,
		CreatingFolder,
		LoadingFile,
		SavingToFile,
		Playing,
		Paused,
		Quick,
		Heavy,
		Normal,
		Parry
	};

	static void addEvent(const Logger::EventType type, const Logger::Action action, const std::string message);
	static void clearLogs();
private:
	static void fileOutput(std::string log);
	static std::string getTime();
	static std::string Logger::getDate();
};