#pragma once

#include "scenes/scene_tutorial_main.h"
#include "scenes/scene_menu.h"
#include "scenes/scene_combat.h"

extern MenuScene menu;
extern TutorialMain tutorialMain;
extern Combat combat;

static std::vector<std::shared_ptr<Entity>> passingEnts;