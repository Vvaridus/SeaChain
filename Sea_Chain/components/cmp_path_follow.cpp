#include "cmp_path_follow.h"
#include "../helpers/astar.h"
#include <LevelSystem.h>
#include <maths.h>

using namespace sf;
using namespace std;

void PathfindingComponent::update(double dt) {
	_elapsed += dt;
	if (_elapsed >= 0.4) {
		_elapsed = 0.0;
		if (_index < _path.size()) {
			auto pos = Vector2i(_parent->getPosition().x / 64, (_parent->getPosition().y - 120) / 64);
			// if he has reached the new tile, increment the index so he goes to the new tile
			if (pos.x == _path[_index].x && pos.y == _path[_index].y)
				++_index;
			else {
				// floating points are evil, this is based all off of entities position
				//float new_x = ls::getOffset().x + _path[_index].x * ls::getTileSize();
				//float new_y = ls::getOffset().y + _path[_index].y * ls::getTileSize();

				//Vector2f direction(0, 0);
				//if (_parent->getPosition().x < new_x)
				//	direction.x += 1.f;
				//else if (_parent->getPosition().x > new_x)
				//	direction.x -= 1.f;
				//else if (_parent->getPosition().y < new_y)
				//	direction.y += 1.f;
				//else if (_parent->getPosition().y > new_y)
				//	direction.y -= 1.f;

				// integers are good, this is based all off of tile position. (Floating evil)
				Vector2f direction(0, 0);
				if (pos.x < _path[_index].x)
					direction.x += 1.f;
				else if (pos.x > _path[_index].x)
					direction.x -= 1.f;
				else if (pos.y < _path[_index].y)
					direction.y += 1.f;
				else if (pos.y > _path[_index].y)
					direction.y -= 1.f;

				auto movement = _parent->GetCompatibleComponent<BasicAiMovementComponent>()[0];
				movement->setDirection(direction);
			}
		}
	}
}

PathfindingComponent::PathfindingComponent(Entity* p)
	: Component(p) {}


void PathfindingComponent::setPath(std::vector<sf::Vector2i>& path) {
	_index = 0;
	_path = path;
}

void BasicAiMovementComponent::setDirection(sf::Vector2f direction) {
	_direction = direction;
}

void BasicAiMovementComponent::update(double dt) {
	move(normalize(_direction) * _speed * (float)dt);
}

BasicAiMovementComponent::BasicAiMovementComponent(Entity* p) : _direction(Vector2f(0.f, 0.f)), _speed(100.f), Component(p) { }

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
