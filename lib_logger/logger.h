#include <string>
#include <map>

class Logger {
public:
	static enum class EventType {
		Engine,
		Tile_Loader,
		Tag,
		Scene,
		Entity
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
		Entity_Deleted
	};

	static void addEvent(const Logger::EventType type, const Logger::Action action, const std::string message);
private:
	static std::string getTime();
};