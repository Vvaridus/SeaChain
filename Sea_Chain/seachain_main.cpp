#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
#include "gameData.h"

using namespace std;

MenuScene menu;
TutorialMain tutorialMain;

int main() {
	auto ins = Singleton::getInstance();
	string value = "Testing";
	ins->setValue(&value);

	Engine::Start(2000, 980, "Sea Chain", &menu);
}