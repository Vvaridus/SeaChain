#include "logger.h"
#include <iostream>
#include <chrono>
#include <string>

std::string Logger::getTime() {
	time_t t;
	struct tm* tt;
	time(&t);
	tt = localtime(&t);
	std::string currentTime = asctime(tt);
	currentTime.pop_back();

	return currentTime;
}

void Logger::sceneLoading(const char *className) {
	std::cout << getTime() << ": Scene \"" << className << "\" Loading" << std::endl;
}

void Logger::sceneUnloading(const char* className) {
	std::cout << getTime() << ": Scene \"" << className << "\" Unloaded" << std::endl;
}

void Logger::sceneLoaded(const char* className) {
	std::cout << getTime() << ": Scene \"" << className << "\" Loaded" << std::endl;
}

void Logger::sceneUpdating(const char* className, double dt) {
	std::cout << getTime() << ": Scene \"" << className << "\" Updating at: " << dt << std::endl;
}

