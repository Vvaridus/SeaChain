#include "scene_tutorial_main.h"
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
#include <system_resources.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> enemy;
sf::Music music;
int volumeMusic = 50;

void TutorialMain::Load() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
    ls::loadLevelFile("resources/map.txt", 54);

<<<<<<< Updated upstream
    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 54.f);
    ls::setOffset(Vector2f(0, ho));
=======
	music.openFromFile("resources/sound/Pirate_1.wav");
	music.setVolume(volumeMusic);
	music.play();
	music.setLoop(true);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 54.f);
	ls::setOffset(Vector2f(0, ho));
>>>>>>> Stashed changes

    // Create player
    {
        player = makeEntity();
        player->addTag("player");
        player->setPosition(Vector2f((Engine::getWindowSize().x / 2) - 100, Engine::getWindowSize().y / 2));

        auto s = player->addComponent<ShapeComponent>();
        s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
        s->getShape().setFillColor(sf::Color::Magenta);
        s->getShape().setOrigin(10.f, 15.f);

        auto b = player->addComponent<BasicMovementComponent>();
        b->setSpeed(400.f);
    }


    // Create enemies
    {
        enemy = makeEntity();
        enemy->addTag("enemy");
        enemy->setPosition(Vector2f(100, 100));

        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(25);
        s->getShape().setFillColor(sf::Color::Green);
        s->getShape().setOrigin(12.5, 12.5);
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
  if (length(player->getPosition() - enemy->getPosition()) < 50) {
      auto ins = Singleton::getInstance();
      auto value = ins->getValue();
      cout << *value;
  }

  Scene::Update(dt);
}

void TutorialMain::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
