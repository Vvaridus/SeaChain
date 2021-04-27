#pragma once

#include "engine.h"
#include "LevelSystem.h"

class TutorialMain : public Scene {
public:
  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;

  void createTexture(std::string path, sf::IntRect bounds, ls::TILES tile);
};
