#include "scene_tutorial_main.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_basic_movement.h"
#include <logger.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
vector<shared_ptr<Entity>> enemies;

void TutorialMain::Load() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
    ls::loadLevelFile("resources/map.txt", 54);

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
    {
        player = makeEntity();
        player->setPosition(Vector2f(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 2));

        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
        s->getShape().setFillColor(Color::Magenta);
        s->getShape().setOrigin(10.f, 15.f);

        auto b = player->addComponent<BasicMovementComponent>();
    }

    // Create enemies
    {
         
    }

    //Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");

    setLoaded(true);
}

void TutorialMain::UnLoad() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void TutorialMain::Update(const double& dt) {
  if (ls::getTileAt(player->getPosition()) == ls::END) {

    //Engine::ChangeScene((Scene*)&level2);
  }

  Scene::Update(dt);
}

void TutorialMain::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
