#include "cmp_basic_movement.h"
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include <maths.h>

void BasicMovementComponent::update(double dt) {
	sf::Vector2f direction(0.0f, 0.0f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		direction.x -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		direction.x += 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		direction.y -= 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		direction.y += 1.0f;
	}

	move(normalize(direction) * _speed * (float)dt);
}

BasicMovementComponent::BasicMovementComponent(Entity* p) : _speed(100.0f), Component(p) { }

bool BasicMovementComponent::validMove(const sf::Vector2f& pos) {
	if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x || pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y) {
		return false;
	}
	return true;
}

void BasicMovementComponent::move(const sf::Vector2f& p) {
	auto new_pos = _parent->getPosition() + p;
	if (validMove(new_pos)) {
		_parent->setPosition(new_pos);
	}
}

void BasicMovementComponent::move(float x, float y) {
	move(sf::Vector2f(x, y));
}