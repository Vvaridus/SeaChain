#pragma once
#include <ecm.h>
#include "../helpers/steering.h"


struct SteeringOutput
{
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
    Seek(Entity *character, Entity *target, float maxSpeed): _character(character), _target(target), _maxSpeed(maxSpeed){}
    SteeringOutput getSteering() const noexcept;
};

class Chase : public SteeringBehaviour {
private:
    Entity* _character;
    Entity* _target;
    float _maxSpeed;
public:
    Chase() = delete;
    Chase(Entity* character, Entity* target, float maxSpeed) : _character(character), _target(target), _maxSpeed(maxSpeed) {}
    SteeringOutput getSteering() const noexcept;
};

class SteeringComponent : public Component {
protected:
    Seek _seek;
    Chase _chase;
    Entity* _player;
    bool validMove(const sf::Vector2f&) const;
public:
    void update(double) override;
    void move(const sf::Vector2f&);
    void move(float x, float y);
    void render() override { }
    explicit SteeringComponent(Entity* p, Entity* player);
    SteeringComponent() = delete;
};