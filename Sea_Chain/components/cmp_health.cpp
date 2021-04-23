#include "cmp_health.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HealthComponent::update(double dt) {
	if (_health <= 0) {
		_parent->setForDelete();
	}
}

HealthComponent::HealthComponent(Entity* p)
	: Component(p), _health(100), _maxHealth(100) {}

float const HealthComponent::getHealth() const
{
	return _health;
}

void HealthComponent::setHealth(float hp)
{
	_health = hp;
}

float const HealthComponent::getMaxHealth() const
{
	return _maxHealth;
}

void HealthComponent::setMaxHealth(float hp)
{
	_maxHealth = hp;
}

void HealthComponent::setAll(float hp, float max)
{
	_health = hp;
	_maxHealth = max;
}

