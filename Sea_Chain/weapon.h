#pragma once
#include <memory>
#include "item.h"
#include <string>

class Weapon : public Item {
public:
	const int getDamageMin() const;
	const int getDamageMax() const;
	const int getDamage() const;
	const int getUses() const;
	void setUses(int);

	Weapon(std::string itemID, int id, Item::Quality quality, int minDamage, int maxDamage, int uses) : Item(itemID, id, quality), damageMax(minDamage), damageMin(maxDamage), usesLeft(uses) {}
	virtual ~Weapon();

protected:
	int damageMin;
	int damageMax;
	int usesLeft;
};