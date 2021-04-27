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

static sf::Music music;
static int volumeMusic = 100;

static Image playerImage;
static Texture playerTexture;
static IntRect playerRect = IntRect(64, 0, 64, 64);


void TutorialMain::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
	ls::loadLevelFile("resources/map.txt", 64);

	playerImage.loadFromFile("resources/SeaChainPlayer.png");
	playerTexture.loadFromImage(playerImage);

	music.openFromFile("resources/sound/Pirate_1.wav");
	music.setVolume(volumeMusic);
	music.play();
	music.setLoop(true);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * 64.f);
	ls::setOffset(Vector2f(0, ho));

	createTexture("resources/SeaChainWorldTilesv.png", IntRect(0, 0, 64, 64), ls::GRASS);
	createTexture("resources/SeaChainWorldTilesv.png", IntRect(64, 0, 64, 64), ls::SAND);
	createTexture("resources/SeaChainWorldTilesv.png", IntRect(128, 0, 64, 64), ls::DIRT);
	createTexture("resources/SeaChainWorldTilesv.png", IntRect(192, 0, 64, 64), ls::STONE);
	createTexture("resources/SeaChainWorldTilesv.png", IntRect(256, 0, 64, 64), ls::WATER);
			
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
			auto s = player->addComponent<SpriteComponent>();
			s->getSprite().setTexture(playerTexture);
			s->getSprite().setTextureRect(playerRect);
			s->getSprite().setOrigin(32.f, 32.f);
			auto b = player->addComponent<BasicMovementComponent>();
			b->setSpeed(120.f);
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
	bool changingScenes = false;
	if (length(player->getPosition() - enemy->getPosition()) < 50) {
		auto ins = Data::getInstance();
		ins->setPlayer(player);
		changingScenes = true;

		Engine::ChangeScene(&combat);
	}

	if (!changingScenes)
	{
		auto s = player->GetCompatibleComponent<SpriteComponent>()[0];
		static sf::Clock clock;
		float elapsed = clock.getElapsedTime().asSeconds();

		//PLAYER ANIMATION FOR UP
		if (Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			if (elapsed > 0.2f)
			{
				if (playerRect.left == 128)
				{
					playerRect.top = 192;
					playerRect.left = 64;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
				else
				{
					playerRect.top = 192;
					playerRect.left += 64;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
			}
		}
		//PLAYER ANIMATION FOR DOWN
		else if (Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			if (elapsed > 0.2f)
			{
				if (playerRect.left == 128)
				{
					playerRect.top = 0;
					playerRect.left = 64;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
				else
				{
					playerRect.top = 0;
					playerRect.left += 64;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
			}
		}
		//PLAYER ANIMATION FOR LEFT
		else if (Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (elapsed > 0.15f)
			{
				if (playerRect.left == 128)
				{
					playerRect.top = 128;
					playerRect.left = 0;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
				else
				{
					playerRect.top = 128;
					playerRect.left += 64;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
			}
		}
		//PLAYER ANIMATION FOR RIGHT
		else if (Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (elapsed > 0.15f)
			{
				if (playerRect.left == 128)
				{
					playerRect.top = 64;
					playerRect.left = 0;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
				else
				{
					playerRect.top = 64;
					playerRect.left += 64;
					s->getSprite().setTextureRect(playerRect);
					clock.restart();
				}
			}
		}

		s->getSprite().setTextureRect(playerRect);
		Scene::Update(dt);
	}
}

void TutorialMain::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void TutorialMain::createTexture(std::string path, sf::IntRect bounds, ls::TILES tile)
{
	//Tile Texture
	Texture worldSpriteSheet;
	worldSpriteSheet.loadFromFile(path, bounds);

	shared_ptr<Texture> worldSheet = make_shared<Texture>(worldSpriteSheet);
	auto tiles = ls::findTiles(tile);
	for (auto t : tiles) {		
		auto pos = ls::getTilePosition(t);
		auto e = makeEntity();
		e->setPosition(pos);
		auto t = e->addComponent<SpriteComponent>();
		t->setTexure(worldSheet);
	}
}
