#include "weapon.h"
#include <iostream>
#include <random>

const int Weapon::getDamageMin() const {
	return damageMin; // return min damage
}

const int Weapon::getDamageMax() const {
	return damageMax; // return max damage
}

const int Weapon::getDamage() const {
	std::random_device dev;
	std::default_random_engine engine(dev());
	std::uniform_int_distribution<int> damage(damageMin, damageMax);

	return damage(engine); // return a random amount of damage
}

const int Weapon::getUses() const {
	return usesLeft; // return the amount of uses
}

const float Weapon::getCrit() const
{
	return 20.f;
}

const float Weapon::getParry() const
{
	return 70.0f;
}

void Weapon::setUses(int uses) {
	usesLeft = uses; // set uses
}

Weapon::Weapon(std::string itemID, Item::Quality quality, int minDamage, int maxDamage, int uses, float crit, float parry) : Item(itemID, quality), damageMax(maxDamage), damageMin(minDamage), usesLeft(uses), critChance(crit), parryChance(parry) {}

Weapon::~Weapon()
{
}