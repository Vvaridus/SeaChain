#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;
public:
  SpriteComponent() = delete;

  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
  void render() override;

  sf::Sprite& getSprite() const;
  std::unique_ptr<sf::FloatRect> getBounds();

  void setOrigin(sf::Vector2f orig);
  void setTexure(std::shared_ptr<sf::Texture> tex);
  void setScaling(sf::Vector2f scale);
};

class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;
  // sf::Shape _shape;

public:
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  void render() override;

  sf::Shape& getShape() const;
  std::unique_ptr<sf::FloatRect> getBounds();

  void setOrigin(sf::Vector2f orig);
  template <typename T, typename... Targs> void setShape(Targs... params) {
    _shape.reset(new T(params...));
  }
};