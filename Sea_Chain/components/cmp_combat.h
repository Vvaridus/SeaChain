#pragma once

#include <ecm.h>
#include <SFML/Graphics/Font.hpp>

class CombatComponent : public Component {
public:
	CombatComponent() = delete;
  explicit CombatComponent(Entity* p);
  void update(double dt) override;

  void render() override;

  ~CombatComponent() override = default;

  void SetText(const std::string& str);

protected:
  //std::shared_ptr<sf::Font> _font;
  //std::string _string;
  //sf::Text _text;
};
