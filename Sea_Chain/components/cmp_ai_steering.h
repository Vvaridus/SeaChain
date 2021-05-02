#pragma once
#include <ecm.h>
#include "../helpers/steering.h"

class SteeringComponent : public Component {
protected:
    Seek _seek;
    Flee _flee;
    Entity* _player;
    bool validMove(const sf::Vector2f&) const;
    sf::Vector2f _direction;
public:
    void update(double) override;
    void move(const sf::Vector2f&);
    void move(float x, float y);
    void render() override { }
    explicit SteeringComponent(Entity* p, Entity* player);
    SteeringComponent() = delete;
};