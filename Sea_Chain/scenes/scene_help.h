#pragma once

#include "engine.h"

class HelpScene : public Scene {
public:
	HelpScene() = default;
	~HelpScene() override = default;

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Nullify();
};
