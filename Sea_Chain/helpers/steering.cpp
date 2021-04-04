#include "../helpers/steering.h"

SteeringOutput Seek::getSteering() const noexcept {
	SteeringOutput steering;
	steering.direction = _target->getPosition() - _character->getPosition();
	steering.direction *= _maxSpeed;
	steering.rotation = 0.0f;
	return steering;
}

SteeringOutput Flee::getSteering() const noexcept {
	SteeringOutput steering;
	steering.direction = _character->getPosition() - _target->getPosition();
	steering.direction *= _maxSpeed;
	steering.rotation = 0.0f;
	return steering;
}