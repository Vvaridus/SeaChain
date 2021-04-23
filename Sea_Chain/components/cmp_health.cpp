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
	: Component(p), _player(_parent->scene->ents.find("player")[0]), _health(100) {}

float const HealthComponent::getHealth() const
{
	return _health;
}

void HealthComponent::setHealth(float hp)
{
	_health = hp;
}
