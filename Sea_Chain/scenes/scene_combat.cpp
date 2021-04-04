#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "scene_combat.h"
#include <logger.h>

using namespace std;
using namespace sf;

shared_ptr<Entity> _player;
shared_ptr<Entity> _enemy;

void Combat::Load() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
    ls::loadLevelFile("resources/combat.txt", 54);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 54.f);
    ls::setOffset(Vector2f(0, ho));

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    
    _player = this->ents.find("player")[0];
    _enemy = this->ents.find("enemy")[0];
    
    _player->setPosition(Vector2f(100, 100));
    _enemy->setPosition(Vector2f(200, 100));

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
