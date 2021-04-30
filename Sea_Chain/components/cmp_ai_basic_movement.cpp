#include "cmp_ai_basic_movement.h"
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include <maths.h>
#include <LevelSystem.h>
#include "../gameData.h"

void BasicAiMovementComponent::update(double dt) {
	auto ins = Data::getInstance();
	sf::Vector2f direction(0.0f, 0.0f);

	move(normalize(direction) * _speed * (float)dt);
}

BasicAiMovementComponent::BasicAiMovementComponent(Entity* p) : _speed(100.0f), Component(p) { }

bool BasicAiMovementComponent::validMove(const sf::Vector2f& pos) {
	if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x || pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y)
		return false;
	else if (ls::getTileAt(pos) == ls::WATER)
	{
		return false;
	}
	else
		return true;
}

void BasicAiMovementComponent::move(const sf::Vector2f& p) {
	auto new_pos = _parent->getPosition() + p;
	if (validMove(new_pos)) {
		_parent->setPosition(new_pos);
	}
}

void BasicAiMovementComponent::move(float x, float y) {
	move(sf::Vector2f(x, y));
}

void BasicAiMovementComponent::setSpeed(float speed) {
	_speed = speed;
}

float BasicAiMovementComponent::getSpeed() {
	return _speed;
}