#pragma once
#include <memory>
#include <string>
#include "engine.h"

class Singleton
{
private:
	static std::shared_ptr<Singleton> instance;
	static std::shared_ptr<Entity> player;

public:
	Singleton();
	static std::shared_ptr<Singleton> getInstance();
	static std::shared_ptr<Entity> getPlayer();
	static void setPlayer(std::shared_ptr<Entity> p);
};