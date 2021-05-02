#include "cmp_ai_steering.h"
#include <SFML/Window/Keyboard.hpp>
#include <engine.h>
#include <maths.h>
#include "cmp_sprite.h"

#define PI 3.14159265

using namespace sf;
using namespace std;

void SteeringComponent::update(double dt) {
	if (length(_parent->getPosition() - _player->getPosition()) > 100.0f) {
		auto output = _seek.getSteering();
		_direction = output.direction;
		float angle = atan2f(_parent->getPosition().y - _player->getPosition().y, _parent->getPosition().x - _player->getPosition().x);
		angle = angle * 360 / PI;
		move(output.direction * (float)dt);

		auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];
		static IntRect enemyRect = IntRect(0, 0, 64, 64);
		static sf::Clock clock;
		float elapsed = clock.getElapsedTime().asSeconds();
		
		//Enemy ANIMATION FOR UP
		if (angle > 50 && angle < 250)
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
		else if (angle < -50 && angle > -240)
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
		else if (angle > -35 && angle < 60)
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
		else if (angle > -360 || angle > 300)
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


	

	else if (length(_parent->getPosition() - _player->getPosition()) < 50.0f) {
		auto output = _flee.getSteering();
		_direction = output.direction;
		float angle = atan2f(_parent->getPosition().y - _player->getPosition().y, _parent->getPosition().x - _player->getPosition().x);
		angle = angle * 360 / PI;
		move(output.direction * (float)dt);

		auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];
		static IntRect enemyRect = IntRect(0, 0, 64, 64);
		static sf::Clock clock;
		float elapsed = clock.getElapsedTime().asSeconds();


		//Enemy ANIMATION FOR UP
		if (angle > 50 && angle < 250)
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
		else if (angle < -50 && angle > -240)
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
		else if (angle > 0 && angle < 60)
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
		else if (angle > -360 || angle > 300)
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