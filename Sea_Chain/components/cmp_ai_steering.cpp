#include "cmp_ai_steering.h"
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include <maths.h>

void SteeringComponent::update(double dt) {
	if (length(_parent->getPosition() - _player->getPosition()) > 100.0f) {
		auto output = _seek.getSteering();
		move(output.direction * (float)dt);
	}

	else if (length(_parent->getPosition() - _player->getPosition()) < 50.0f) {
		auto output = _flee.getSteering();
		move(output.direction * (float)dt);
	}
}

SteeringComponent::SteeringComponent(Entity* p, Entity* player) : _player(player), _seek(Seek(p, player, 10.0f)), _flee(Flee(p, player, 10.0f)), Component(p) { }

bool SteeringComponent::validMove(const sf::Vector2f& pos) const {
	if (pos.x < 0.0f || pos.x > Engine::GetWindow().getSize().x || pos.y < 0.0f || pos.y > Engine::GetWindow().getSize().y) {
		return false;
	}
	return true;
}

void SteeringComponent::move(const sf::Vector2f& p) {
	auto new_pos = _parent->getPosition() + p;
	if (validMove(new_pos)) {
		_parent->setPosition(new_pos);
	}
}

void SteeringComponent::move(float x, float y) {
	move(sf::Vector2f(x, y));
}