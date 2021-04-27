#pragma once

#include "engine.h"

class MenuScene : public Scene {
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Nullify();
};
