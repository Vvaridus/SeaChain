#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
TutorialMain tutorialMain;
CombatScene combat;

int main() {
	Engine::Start(2000, 980, "Sea Chain", &menu);
}
