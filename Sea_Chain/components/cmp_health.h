#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HealthComponent : public Component {
protected:
	float _health;
	float _maxHealth;

public:
	void update(double dt) override;
	void render() override {}
	explicit HealthComponent(Entity* p);
	HealthComponent() = delete;

	float const getHealth() const;
	void setHealth(float hp);
	float const getMaxHealth() const;
	void setMaxHealth(float hp);
	void setAll(float hp, float max);
};
