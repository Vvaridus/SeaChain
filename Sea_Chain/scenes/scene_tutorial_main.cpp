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
#include "../components/cmp_health.h"
#include <SFML/Audio/Music.hpp>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> enemy;
sf::Music music;
int volumeMusic = 50;

void TutorialMain::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
	ls::loadLevelFile("resources/map.txt", 54);

	music.openFromFile("resources/sound/Pirate_1.wav");
	music.setVolume(volumeMusic);
	music.play();
	music.setLoop(true);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 54.f);
	ls::setOffset(Vector2f(0, ho));

	// Create player
	{
		auto ins = Data::getInstance();
		std::shared_ptr<HealthComponent> h;

		if (ins->getPlayer() == nullptr)
		{
			ins->setPlayer(makeEntity());
			player = ins->getPlayer();
			player->addTag("player");
			player->setPosition(Vector2f((Engine::getWindowSize().x / 2), Engine::getWindowSize().y / 2));

			h = player->addComponent<HealthComponent>();

			auto s = player->addComponent<ShapeComponent>();
			s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
			s->getShape().setFillColor(sf::Color::Magenta);
			s->getShape().setOrigin(10.f, 15.f);

			auto b = player->addComponent<BasicMovementComponent>();
			b->setSpeed(600.f);

			player->addComponent<InventoryComponent>();
		}
		else
		{
			player = ins->getPlayer();
			player->setPosition(Vector2f((Engine::getWindowSize().x / 2), Engine::getWindowSize().y / 2));

			cout << player->getPosition() << endl;

			// Add the entity back to the list to be rendered, it was removed earlier.
			tutorialMain.ents.list.push_back(player);
		}


		h = player->GetCompatibleComponent<HealthComponent>()[0];
	}


	// Create enemies
	{
		enemy = makeEntity();
		enemy->addTag("enemy");
		enemy->setPosition(Vector2f(432, 100));

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
		auto ins = Data::getInstance();
		ins->setPlayer(player);

		Engine::ChangeScene(&combat);
	}

	Scene::Update(dt);
}

void TutorialMain::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
