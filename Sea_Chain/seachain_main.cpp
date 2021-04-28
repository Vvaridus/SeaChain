#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
TutorialMain tutorialMain;
CombatScene combat;

int main() {
	Engine::Start(1920, 1080, "Sea Chain", &tutorialMain);
}
