#pragma once

#include "engine.h"
#include "LevelSystem.h"

class TutorialMain : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void checkEventPresses(const double& dt, bool& changingScenes);

  void updateHealthBars(const double& dt, bool changingScenes);

  void Render() override;

  void createTexture(std::string path, sf::IntRect bounds, std::vector<sf::Vector2ul> tiles, std::string tag);

  int randomNumber(int min, int max);

  void Nullify();
};
