#include "gameData.h"
#include <string>

std::shared_ptr<Entity> Data::player;
std::shared_ptr<Data> Data::instance;

std::shared_ptr<Data> Data::getInstance() {
	if (instance == nullptr)
		instance = std::make_shared<Data>();

	return instance;
}

std::shared_ptr<Entity> Data::getPlayer() {
	return player;
}

void Data::setPlayer(std::shared_ptr<Entity> p) {
	player = std::move(p);
}

Data::Data() {}
