#include "gameData.h"
#include <string>

std::string* Singleton::value;
Singleton* Singleton::instance;

Singleton* Singleton::getInstance() {
	if (instance == nullptr)
		instance = new Singleton();

	return instance;
}

std::string* Singleton::getValue() {
	return value;
}

void Singleton::setValue(std::string*v) {
	value = std::move(v);
}

Singleton::Singleton() {}

Singleton::~Singleton() {
	delete instance;
	delete value;
}