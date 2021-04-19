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
	void setUses(int); // set the uses of the item.

	Weapon(std::string itemID, int id, Item::Quality quality, int minDamage, int maxDamage, int uses) : Item(itemID, id, quality), damageMax(minDamage), damageMin(maxDamage), usesLeft(uses) {}
	virtual ~Weapon();

protected:
	int damageMin;
	int damageMax;
	int usesLeft; // change this to a pointer
};