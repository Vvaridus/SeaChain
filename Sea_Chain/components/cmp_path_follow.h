#pragma once
#include <ecm.h>

class PathfindingComponent : public Component {
protected:
	std::vector<sf::Vector2i> _path;
	size_t _index = 0;
	double _elapsed = 0.0;

public:
  void update(double dt) override;
  void render() override {}
  void setPath(std::vector<sf::Vector2i>& path);
  explicit PathfindingComponent(Entity* p);
  PathfindingComponent() = delete;
};

class BasicAiMovementComponent : public Component {
protected:
	float _speed;
	sf::Vector2f _direction;
	bool validMove(const sf::Vector2f&);

public:
	void setSpeed(float speed);
	void setDirection(sf::Vector2f direction);
	float getSpeed();
	void update(double) override;
	void move(const sf::Vector2f&);
	void move(float x, float y);
	void render() override {};
	explicit BasicAiMovementComponent(Entity* p);
	BasicAiMovementComponent() = delete;
};
