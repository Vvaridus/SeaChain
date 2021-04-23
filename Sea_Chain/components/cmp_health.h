#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HealthComponent : public Component {
protected:
	std::weak_ptr<Entity> _player;
	float _health;

public:
	void update(double dt) override;
	void render() override {}
	explicit HealthComponent(Entity* p);
	HealthComponent() = delete;

	float const getHealth() const;
	void setHealth(float hp);
};
