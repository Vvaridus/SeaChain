#pragma once
#include <memory>
#include <string>

class Singleton
{
private:
	static Singleton* instance;
	static std::string* value;
	Singleton();
	~Singleton();

public:
	static Singleton* getInstance();
	static std::string* getValue();
	static void setValue(std::string *v);
};