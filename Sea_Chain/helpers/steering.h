#pragma once
#include <engine.h>

struct SteeringOutput {
	sf::Vector2f direction;
	
	float rotation;
};

class SteeringBehaviour {
public:
	virtual ~SteeringBehaviour() = default;
	virtual SteeringOutput getSteering() const noexcept = 0;
};

class Seek : public SteeringBehaviour {
private:
	Entity* _character;
	Entity* _target;
	float _maxSpeed;
public:
	Seek() = delete;
	Seek(Entity *character, Entity *target, float maxSpeed) : _character(character), _target(target), _maxSpeed(maxSpeed) { }
	SteeringOutput getSteering() const noexcept;
};

class Flee : public SteeringBehaviour {
private:
	Entity* _character;
	Entity* _target;
	float _maxSpeed;
public:
	Flee() = delete;
	Flee(Entity *character, Entity *target, float maxSpeed) : _character(character), _target(target), _maxSpeed(maxSpeed) { }
	SteeringOutput getSteering() const noexcept;
};