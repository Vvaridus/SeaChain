#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "scene_combat.h"

using namespace std;
using namespace sf;

void Combat::Load() {
    cout << " Scene < Load" << endl;
    ls::loadLevelFile("resources/combat.txt", 54);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Combat::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  ls::unload();
  Scene::UnLoad();
}

void Combat::Update(const double& dt) {
  Scene::Update(dt);
}

void Combat::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
