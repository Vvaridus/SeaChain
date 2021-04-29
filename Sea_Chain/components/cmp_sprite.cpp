
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;


void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
}



void SpriteComponent::setOrigin(sf::Vector2f orig) {
    _sprite->setOrigin(orig);    
}

void SpriteComponent::setScaling(sf::Vector2f scale) {
    _sprite->setScale(scale);
}

std::unique_ptr<sf::FloatRect> SpriteComponent::getBounds() {
    //return std::make_unique<sf::FloatRect>(_sprite->getLocalBounds());
    return std::make_unique<sf::FloatRect>(_sprite->getGlobalBounds());
}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }

bool SpriteComponent::getVisibility() {
    return _visible;
}

void SpriteComponent::setVisibility(bool visible) {
    _visible = visible;
}

SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()), _visible(true) {}

void SpriteComponent::update(double dt) {
  _sprite->setPosition(_parent->getPosition());
  _sprite->setRotation(_parent->getRotation());
}

void SpriteComponent::render() { 
    if(_visible)
        Renderer::queue(_sprite.get()); 
}

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition());
  _shape->setRotation(_parent->getRotation());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

std::unique_ptr<sf::FloatRect> ShapeComponent::getBounds() {
    //return std::make_unique<sf::FloatRect>(_sprite->getLocalBounds());
    return std::make_unique<sf::FloatRect>(_shape->getGlobalBounds());
}

void ShapeComponent::setOrigin(sf::Vector2f orig) {
    _shape->setOrigin(orig);
}

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

