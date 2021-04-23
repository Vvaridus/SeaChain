#pragma once
#include <memory>
#include "item.h"
#include <string>

class Weapon : public Item {
public:
	const int getDamageMin() const; // Min damage weapon can do.
	const int getDamageMax() const; // max damage weapon can do.
	const int getDamage() const; // get a random damage.
	const int getUses() const; // get the amount of uses left of item.
	const float getCrit() const; // get the chance of crits
	const float getParry() const; // get the chance of being able to parry.
	void setUses(int); // set the uses of the item.

	Weapon() { }
	Weapon(std::string itemID, Item::Quality quality, int minDamage, int maxDamage, int uses, float crit, float parry);
	virtual ~Weapon();

protected:
	int damageMin;
	int damageMax;
	float critChance;
	float parryChance;
	int usesLeft;
};