#pragma once

#include "engine.h"

class OptionScene : public Scene {
public:
	OptionScene() = default;
	~OptionScene() override = default;

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void updateMusicIndicator();
	void updateSoundIndicator();

	void Nullify();
};
