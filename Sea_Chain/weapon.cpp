#include "weapon.h"

const int Weapon::getDamageMin() const {
	return damageMin;
}

const int Weapon::getDamageMax() const {
	return damageMax;
}

const int Weapon::getDamage() const {
	return rand() % (damageMax - damageMin + 1) + (damageMin);
}

const int Weapon::getUses() const {
	return usesLeft;
}

void Weapon::setUses(int uses) {
	usesLeft = uses;
}

Weapon::~Weapon()
{
}
