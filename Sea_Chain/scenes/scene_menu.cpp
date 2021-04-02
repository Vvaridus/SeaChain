#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../logger.h"

using namespace std;
using namespace sf;

void MenuScene::Load() {
    Logger::sceneLoading(typeid(this).name());

    {
        auto txt = makeEntity();
        auto t = txt->addComponent<TextComponent>(
            "Sea Chain\nPress Space to Start");
    }

    Logger::sceneLoaded(typeid(this).name());
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    //Logger::sceneUpdating(typeid(this).name(), dt);

    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        Engine::ChangeScene(&tutorialMain);
    }

    Scene::Update(dt);
}
