#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "scene_combat.h"
#include "../logger.h"

using namespace std;
using namespace sf;

void Combat::Load() {
    //Logger::sceneLoading(typeid(*this).name());

    ls::loadLevelFile("resources/combat.txt", 54);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    //Logger::sceneLoading(typeid(*this).name());

    setLoaded(true);
}

void Combat::UnLoad() {
    //Logger::sceneUnloading(typeid(*this).name());

  ls::unload();
  Scene::UnLoad();
}

void Combat::Update(const double& dt) {
    //Logger::sceneUpdating(typeid(*this).name(), dt);

  Scene::Update(dt);
}

void Combat::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
