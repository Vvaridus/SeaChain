#include "scene_combat.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_basic_movement.h"
#include <logger.h>
#include "../weapon.h"
#include "../components/cmp_inventory.h"
#include "../gameData.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> enemy;

void CombatScene::Load() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
    ls::loadLevelFile("resources/combat.txt", 54);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 54.f);
    ls::setOffset(Vector2f(0, ho));

    auto ins = Data::getInstance();
    auto p = ins->getPlayer();
    player = move(p);


    //Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");

    setLoaded(true);
}

void CombatScene::UnLoad() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void CombatScene::Update(const double& dt) {
  Scene::Update(dt);
}

void CombatScene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
