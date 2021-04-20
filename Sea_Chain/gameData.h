#pragma once
#include <memory>
#include <string>
#include "engine.h"

class Data
{
private:
	static std::shared_ptr<Data> instance;
	static std::shared_ptr<Entity> player;

public:
	Data();
	static std::shared_ptr<Data> getInstance();
	static std::shared_ptr<Entity> getPlayer();
	static void setPlayer(std::shared_ptr<Entity> p);
};