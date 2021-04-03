#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "scene_combat.h"
#include <logger.h>

using namespace std;
using namespace sf;

void Combat::Load() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
    ls::loadLevelFile("resources/combat.txt", 54);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");
    setLoaded(true);
}

void Combat::UnLoad() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
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
