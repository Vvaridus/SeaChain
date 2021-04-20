#include "gameData.h"
#include <string>

Entity* Singleton::player;
Singleton* Singleton::instance;

Singleton* Singleton::getInstance() {
	if (instance == nullptr)
		instance = new Singleton();

	return instance;
}

Entity* Singleton::getPlayer() {
	return player;
}

void Singleton::setPlayer(Entity* p) {
	player = std::move(p);
}

Singleton::Singleton() {}

Singleton::~Singleton() {
	delete instance;
	delete player;
}