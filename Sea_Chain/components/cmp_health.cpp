#include "cmp_health.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HealthComponent::update(double dt) {
	if (_health <= 0) {
		_parent->setForDelete();
	}
}

// set the default health to 500
HealthComponent::HealthComponent(Entity* p)
	: Component(p), _health(500), _maxHealth(500) {}

// Get the current health
float const HealthComponent::getHealth() const
{
	return _health;
}

// Set the health
void HealthComponent::setHealth(float hp)
{
	_health = hp;
}

// Get the max health
float const HealthComponent::getMaxHealth() const
{
	return _maxHealth;
}

// Set max health
void HealthComponent::setMaxHealth(float hp)
{
	_maxHealth = hp;
}

// Set HP and Max health
void HealthComponent::setAll(float hp, float max)
{
	_health = hp;
	_maxHealth = max;
}

