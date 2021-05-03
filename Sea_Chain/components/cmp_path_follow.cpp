#include "cmp_path_follow.h"
#include "../helpers/astar.h"
#include <LevelSystem.h>
#include <maths.h>
#include "cmp_sprite.h"

using namespace sf;
using namespace std;

void PathfindingComponent::update(double dt) {
	_elapsed += dt;
	if (_elapsed >= 0.4) {
		_elapsed = 0.0;
		if (_index < _path.size()) {
			auto pos = Vector2i(_parent->getPosition().x / ls::getTileSize(), (_parent->getPosition().y - ls::getOffset().y) / ls::getTileSize());
			// if he has reached the new tile, increment the index so he goes to the new tile
			if (pos.x == _path[_index].x && pos.y == _path[_index].y)
				++_index;
			else {
				// floating points are evil, this is based all off of entities screen/world/map position
				// gets stuck in a loop
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

				// integers are good, this is based all off of tile position.
				// get what direction they need to head in
				Vector2f direction(0, 0);
				if (pos.x < _path[_index].x)
					direction.x += 1.f;
				else if (pos.x > _path[_index].x)
					direction.x -= 1.f;
				else if (pos.y < _path[_index].y)
					direction.y += 1.f;
				else if (pos.y > _path[_index].y)
					direction.y -= 1.f;

				// set the direction to move
				auto movement = _parent->GetCompatibleComponent<BasicAiMovementComponent>()[0];
				movement->setDirection(direction);
			}
		}
	}
}

PathfindingComponent::PathfindingComponent(Entity* p)
	: Component(p) {}

// set the path to move through
void PathfindingComponent::setPath(std::vector<sf::Vector2i>& path) {
	_index = 0;
	_path = path;
}

// set the direction to head in
void BasicAiMovementComponent::setDirection(sf::Vector2f direction) {
	_direction = direction;
}

void BasicAiMovementComponent::update(double dt) {
	move(normalize(_direction) * _speed * (float)dt);

	auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];
	static IntRect enemyRect = IntRect(0, 0, 64, 64);
	static sf::Clock clock;
	float elapsed = clock.getElapsedTime().asSeconds();

	//Enemy ANIMATION FOR UP
	if (_direction == Vector2f(0.f, -1.f))
	{
		if (elapsed > 0.2f)
		{
			if (enemyRect.left == 128)
			{
				enemyRect.top = 192;
				enemyRect.left = 64;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
			else
			{
				enemyRect.top = 192;
				enemyRect.left += 64;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
		}
	}
	//Enemy ANIMATION FOR DOWN
	else if (_direction == Vector2f(0.f, 1.f))
	{
		if (elapsed > 0.2f)
		{
			if (enemyRect.left == 128)
			{
				enemyRect.top = 0;
				enemyRect.left = 64;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
			else
			{
				enemyRect.top = 0;
				enemyRect.left += 64;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
		}
	}
	//Enemy ANIMATION FOR LEFT
	else if (_direction == Vector2f(-1.f, 0.f))
	{
		if (elapsed > 0.15f)
		{
			if (enemyRect.left == 128)
			{
				enemyRect.top = 128;
				enemyRect.left = 0;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
			else
			{
				enemyRect.top = 128;
				enemyRect.left += 64;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
		}
	}
	//Enemy ANIMATION FOR RIGHT
	else if (_direction == Vector2f(+1.f, -0.f))
	{
		if (elapsed > 0.15f)
		{
			if (enemyRect.left == 128)
			{
				enemyRect.top = 64;
				enemyRect.left = 0;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
			else
			{
				enemyRect.top = 64;
				enemyRect.left += 64;
				s->getSprite().setTextureRect(enemyRect);
				clock.restart();
			}
		}
	}

	s->getSprite().setTextureRect(enemyRect);
}


BasicAiMovementComponent::BasicAiMovementComponent(Entity* p) : _direction(Vector2f(0.f, 0.f)), _speed(100.f), Component(p) { }

// Check if its a valid move
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
