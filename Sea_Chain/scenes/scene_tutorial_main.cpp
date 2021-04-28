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
#include <random>

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

	auto windowSize = Engine::getWindowSize();
	auto ho = windowSize.y - (ls::getHeight() * 64.f);
	ls::setOffset(Vector2f(0, ho));

	// Allow us to randomly shuffle a vector
	std::random_device rd;
	auto rng = std::default_random_engine{ rd() };

	// create basic world textures
	{
		createTexture("resources/SeaChainWorldTilesv.png", IntRect(0, 0, 64, 64), ls::findTiles(ls::GRASS));
		createTexture("resources/SeaChainWorldTilesv.png", IntRect(64, 0, 64, 64), ls::findTiles(ls::SAND));
		createTexture("resources/SeaChainWorldTilesv.png", IntRect(128, 0, 64, 64), ls::findTiles(ls::DIRT));
		createTexture("resources/SeaChainWorldTilesv.png", IntRect(192, 0, 64, 64), ls::findTiles(ls::STONE));
		createTexture("resources/SeaChainWorldTilesv.png", IntRect(256, 0, 64, 64), ls::findTiles(ls::WATER));
	}

	// add extra detail textures
	{

		// This will draw a tree over GRASS tiles
		// Get a list of all grass tiles and then shuffle the vector
		// get a random numberOfTiles of 0-80% of the vector size
		// then get a random tile from the list and add that to a vector to be set to a tree.
		auto tileList = ls::findTiles(ls::GRASS);
		std::shuffle(std::begin(tileList), std::end(tileList), rng);
		std::vector<sf::Vector2ul> randomTiles;
		int numberOfTiles = randomNumber(0, 0.8 * (tileList.size() - 1));
		for (int i = 0; i < numberOfTiles; i++) {
			int rand = randomNumber(0, tileList.size() - 1);
			randomTiles.push_back(tileList[rand]);
		}

		createTexture("resources/SeaChainWorldTilesv.png", IntRect(192, 64, 64, 64), randomTiles);
		randomTiles.clear();
		tileList.clear();

		// This will draw a rock over STONE tiles
		// Get a list of all stone tiles and then shuffle the vector
		// get a random numberOfTiles of 0-40% of the vector size
		// then get a random tile from the list and add that to a vector to be set to a rock.
		tileList = ls::findTiles(ls::STONE);
		std::shuffle(std::begin(tileList), std::end(tileList), rng);
		numberOfTiles = randomNumber(0, 0.4 * (tileList.size() - 1));
		for (int i = 0; i < numberOfTiles; i++) {
			int rand = randomNumber(0, tileList.size() - 1);
			randomTiles.push_back(tileList[rand]);
		}


		createTexture("resources/SeaChainWorldTilesv.png", IntRect(64, 64, 64, 64), randomTiles);
		randomTiles.clear();
		tileList.clear();

		// This will draw a skeliton on DIRT tiles
		// Get a list of all dirt tiles and then shuffle the vector
		// get a random numberOfTiles of 0-5% of the vector size
		// then get a random tile from the list and add that to a vector to be set to a tree.
		tileList = ls::findTiles(ls::DIRT);
		std::shuffle(std::begin(tileList), std::end(tileList), rng);
		numberOfTiles = randomNumber(0, 0.05 * (tileList.size() - 1));
		for (int i = 0; i < numberOfTiles; i++) {
			int rand = randomNumber(0, tileList.size() - 1);
			randomTiles.push_back(tileList[rand]);
		}

		createTexture("resources/SeaChainWorldTilesv.png", IntRect(0, 64, 64, 64), randomTiles);
		randomTiles.clear();
		tileList.clear();
	}

	// Add bed to map
	{
		auto tileList = ls::findTiles(ls::STONE);
		std::shuffle(std::begin(tileList), std::end(tileList), rng);
		std::vector<sf::Vector2ul> randomTiles;

		Texture bedSheet;
		bedSheet.loadFromFile("resources/Bed.png", IntRect(0, 0, 64, 64));

		shared_ptr<Texture> bedSprite = make_shared<Texture>(bedSheet);

		auto bed = makeEntity();
		bed->addTag("bed");
		bed->setPosition(ls::getTilePosition(tileList[randomNumber(0, tileList.size() - 1)]));
		auto bedSpriteComp = bed->addComponent<SpriteComponent>();
		bedSpriteComp->setTexure(bedSprite);
	}

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
		enemy->setPosition(Vector2f(432, 250));

		auto s = enemy->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(25);
		s->getShape().setFillColor(sf::Color::Green);
		s->getShape().setOrigin(12.5, 12.5);
	}

	// Draw the UI overlay
	{
		// load the sprite
		Texture mainBanner;
		mainBanner.loadFromFile("resources/SeaChainMainBanner.png", IntRect(0, 0, 1920, 1080));
		shared_ptr<Texture> spriteBanner = make_shared<Texture>(mainBanner);
		Texture healthBar;
		healthBar.loadFromFile("resources/SeaChainHealthBar.png", IntRect(0, 0, 224, 33));
		shared_ptr<Texture> spriteHealth = make_shared<Texture>(healthBar);

		// set the position, add a tag, add the sprite component with the texture
		auto banner = makeEntity();
		banner->addTag("mainBanner");
		banner->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto bannerSprite = banner->addComponent<SpriteComponent>();
		bannerSprite->setTexure(spriteBanner);
		bannerSprite->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));

		auto healthBanner = makeEntity();
		healthBanner->addTag("healthUIBar");
		healthBanner->setPosition(Vector2f(256, 49));
		auto healthSprite = healthBanner->addComponent<SpriteComponent>();
		healthSprite->setTexure(spriteHealth);
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

	static sf::Clock bedCooldown;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		// triggertime for a cool down on pressing the key, so it doesn't trigger several times in a second.
		if (bedCooldown.getElapsedTime().asSeconds() > 10)
		{
			// get the inventory, create the weapon, add it and set it to using.
			auto bed = this->ents.find("bed")[0];
			if (length(player->getPosition() - bed->getPosition()) < 50)
			{
				auto ins = Data::getInstance();
				auto player = ins->getPlayer();
				auto health = player->GetCompatibleComponent<HealthComponent>()[0];
				health->setHealth(health->getMaxHealth());
				bedCooldown.restart();
			}
		}
		cout << bedCooldown.getElapsedTime().asSeconds() << endl;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		auto ins = Data::getInstance();
		auto player = ins->getPlayer();
		auto health = player->GetCompatibleComponent<HealthComponent>()[0];
		health->setHealth(5);
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
		updateHealthBars(dt);
		Scene::Update(dt);
	}
}

void TutorialMain::updateHealthBars(const double& dt) {
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto playerHealth = player->GetCompatibleComponent<HealthComponent>()[0];
	float barWidth;

	auto uiBar = this->ents.find("healthUIBar")[0];
	auto spriteComponent = uiBar->GetCompatibleComponent<SpriteComponent>()[0];
	barWidth = max((playerHealth->getHealth() / playerHealth->getMaxHealth()) * spriteComponent->getSprite().getTexture()->getSize().x, 0.f);
	spriteComponent->getSprite().setTextureRect(IntRect(0, 0, static_cast<int>(barWidth), spriteComponent->getBounds()->height));
}

void TutorialMain::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void TutorialMain::createTexture(std::string path, sf::IntRect bounds, std::vector<sf::Vector2ul> tiles)
{
	//Tile Texture
	Texture worldSpriteSheet;
	worldSpriteSheet.loadFromFile(path, bounds);

	shared_ptr<Texture> worldSheet = make_shared<Texture>(worldSpriteSheet);
	for (auto t : tiles) {
		auto pos = ls::getTilePosition(t);
		auto e = makeEntity();
		e->setPosition(pos);
		auto t = e->addComponent<SpriteComponent>();
		t->setTexure(worldSheet);
	}
}

int TutorialMain::randomNumber(int min, int max) {
	// Seed the random_device with the random engine and get a uniform_real_distribution between the min and max value
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> damage(min, max);

	return damage(engine);
}
