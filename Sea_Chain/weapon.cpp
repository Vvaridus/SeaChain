#include "weapon.h"

const int Weapon::getDamageMin() const {
	return damageMin; // return min damage
}

const int Weapon::getDamageMax() const {
	return damageMax; // return max damage
}

const int Weapon::getDamage() const {
	return rand() % (damageMax - damageMin + 1) + (damageMin); // return a random amount of damage
}

const int Weapon::getUses() const {
	return usesLeft; // return the amount of uses
}

void Weapon::setUses(int uses) {
	usesLeft = uses; // set uses
}

Weapon::Weapon(std::string itemID, int id, Item::Quality quality, int minDamage, int maxDamage, int uses) : Item(itemID, id, quality), damageMax(minDamage), damageMin(maxDamage), usesLeft(uses) {}

Weapon::~Weapon()
{
}
