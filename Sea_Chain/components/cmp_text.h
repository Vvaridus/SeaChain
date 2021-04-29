#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class TextComponent : public Component {
public:
  TextComponent() = delete;
  explicit TextComponent(Entity* p, const std::string& str = "");
  void update(double dt) override;

  void render() override;

  ~TextComponent() override = default;

  void SetText(const std::string& str);
  std::string getText();
  void setCharSize(int s);
  void setOrigin(sf::Vector2f o);
  void setPosition(sf::Vector2f p);
  void setRotation(float r);
  void setFillColor(sf::Color c);
  void setStyle(sf::Text::Style s);
  sf::FloatRect getBounds();

protected:
  std::shared_ptr<sf::Font> _font;
  std::string _string;
  sf::Text _text;
};
