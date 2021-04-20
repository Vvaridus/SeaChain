#pragma once
#include <memory>
#include <string>
#include "engine.h"

class Singleton
{
private:
	static Singleton* instance;
	static Entity* player;
	Singleton();
	~Singleton();

public:
	static Singleton* getInstance();
	static Entity* getPlayer();
	static void setPlayer(Entity* p);
};