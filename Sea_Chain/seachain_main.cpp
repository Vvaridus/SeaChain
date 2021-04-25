#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"
<<<<<<< Updated upstream
#include "gameData.h"
=======
#include <SFML/Window/Keyboard.hpp>

>>>>>>> Stashed changes

using namespace std;

MenuScene menu;
TutorialMain tutorialMain;

int main() {
<<<<<<< Updated upstream
	auto ins = Singleton::getInstance();
	string value = "Testing";
	ins->setValue(&value);

	Engine::Start(2000, 980, "Sea Chain", &menu);
}
=======
	Engine::Start(1920, 1080, "Sea Chain", &tutorialMain);

	
}
>>>>>>> Stashed changes
