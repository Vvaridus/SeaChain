#include <string>

class Logger {
public:
	static void sceneLoading(const char*);
	static void sceneLoaded(const char*);
	static void sceneUnloading(const char*);
	static void sceneUpdating(const char*, double dt);
private:
	static std::string getTime();
};