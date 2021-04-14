#pragma once
#include <memory>
#include "item.h"

class Weapon : public Item {
public:
	const int getDamageMin() const;
	const int getDamageMax() const;
	const int getDamage() const;
	const int getUses() const;
	void setUses(int);

	Weapon(Item::ItemType type, Item::Quality quality, int minDamage, int maxDamage, int uses) : Item(type, quality), damageMax(minDamage), damageMin(maxDamage), usesLeft(uses) {}
	virtual ~Weapon();

protected:
	int damageMin;
	int damageMax;
	int usesLeft;
};