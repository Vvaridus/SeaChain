#include "engine.h"
#include "game.h"

using namespace std;

MenuScene menu;
TutorialMain tutorialMain;
CombatScene combat;
OptionScene options;

int main() {
	Engine::Start(1920, 1080, "Sea Chain", &menu);
}
