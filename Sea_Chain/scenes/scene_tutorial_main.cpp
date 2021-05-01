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
#include "../components/cmp_button.h"
#include "../components/cmp_ai_steering.h"
#include "../components/cmp_path_follow.h"
#include "../helpers/astar.h"
#include "../components/cmp_ai_basic_movement.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> enemy;

static Image playerImage;
static Texture playerTexture;
static IntRect playerRect = IntRect(64, 0, 64, 64);
static Image enemyImage;
static Texture enemyTexture;
static IntRect enemyRect = IntRect(0, 0, 64, 64);

static bool pause = false;

std::shared_ptr<ButtonComponent> btnMenu;
std::shared_ptr<ButtonComponent> btnQuit;


void TutorialMain::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");
	ls::loadLevelFile("resources/map.txt", 64);

	playerImage.loadFromFile("resources/textures/SeaChainPlayer.png");
	playerTexture.loadFromImage(playerImage);
	enemyImage.loadFromFile("resources/textures/SeaChainEnemy.png");
	enemyTexture.loadFromImage(enemyImage);

	auto ins = Data::getInstance();
	auto debug = ins->getDebug();

	pause = false;

	auto windowSize = Engine::getWindowSize();
	auto ho = windowSize.y - (ls::getHeight() * 64.f);
	ls::setOffset(Vector2f(0, ho));

	// Allow us to randomly shuffle a vector
	std::random_device rd;
	auto rng = std::default_random_engine{ rd() };

	// create basic world textures
	{
		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(0, 0, 64, 64), ls::findTiles(ls::GRASS), "grass");
		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(64, 0, 64, 64), ls::findTiles(ls::SAND), "sand");
		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(128, 0, 64, 64), ls::findTiles(ls::DIRT), "dirt");
		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(192, 0, 64, 64), ls::findTiles(ls::STONE), "stone");
		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(256, 0, 64, 64), ls::findTiles(ls::WATER), "water");
	}

	// add extra entities rocks, skeletons, trees
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

		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(192, 64, 64, 64), randomTiles, "tree");
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


		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(64, 64, 64, 64), randomTiles, "rock");
		randomTiles.clear();
		tileList.clear();

		// This will draw a skeleton on DIRT tiles
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

		createTexture("resources/textures/SeaChainWorldTilesv.png", IntRect(0, 64, 64, 64), randomTiles, "skeleton");
		randomTiles.clear();
		tileList.clear();
	}

	// Add bed to map
	{
		auto tileList = ls::findTiles(ls::STONE);
		std::shuffle(std::begin(tileList), std::end(tileList), rng);
		std::vector<sf::Vector2ul> randomTiles;

		Texture bedSheet;
		bedSheet.loadFromFile("resources/textures/Bed.png", IntRect(0, 0, 64, 64));

		shared_ptr<Texture> bedSprite = make_shared<Texture>(bedSheet);

		auto bed = makeEntity();
		bed->addTag("bed");
		bed->setPosition(ls::getTilePosition(tileList[randomNumber(0, tileList.size() - 1)]));
		auto bedSpriteComp = bed->addComponent<SpriteComponent>();
		bedSpriteComp->setTexure(bedSprite);
	}

	createWaterBlend();

	// Create player
	{
		auto ins = Data::getInstance();

		if (ins->getPlayer() == nullptr)
		{
			ins->setPlayer(makeEntity());
			player = ins->getPlayer();
			player->addTag("player");
			player->setPosition(Vector2f((Engine::getWindowSize().x / 2), Engine::getWindowSize().y / 2));
			player->addComponent<HealthComponent>();
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
		// Ohhh whats this?
		{
			if (ins->getBiscuit()) {
				Texture spritesheet;
				spritesheet.loadFromFile("resources/textures/ShipBiscuit.png", IntRect(0, 0, 64, 64));
				shared_ptr<Texture> spriteBiscuit = make_shared<Texture>(spritesheet);				

				auto p = ins->getPlayer();
				auto sprite = p->GetCompatibleComponent<SpriteComponent>()[0];
				sprite->setTexure(spriteBiscuit);
				sprite->getSprite().setTextureRect(IntRect(0, 0, 64, 64));
			}
			else {
				auto p = ins->getPlayer();
				auto sprite = p->GetCompatibleComponent<SpriteComponent>()[0];
				sprite->getSprite().setTexture(playerTexture);
				sprite->getSprite().setTextureRect(playerRect);
			}
		}
	}

	// Create enemies
	{
		enemy = nullptr;
		enemy = makeEntity();
		enemy->addTag("enemy");
		enemy->setPosition(Vector2f(384, 376));
		//THIS MUST COME BEFORE MOVEMENT!
		enemy->addComponent<PathfindingComponent>();
		enemy->addComponent<BasicAiMovementComponent>();

		auto s = enemy->addComponent<SpriteComponent>();
		s->getSprite().setTexture(enemyTexture);
		s->getSprite().setTextureRect(enemyRect);
		s->getSprite().setOrigin(32.f, 32.f);		
	}

	random_device dev;
	default_random_engine engine(dev());
	uniform_real_distribution<float> x_dist(0.0f, Engine::GetWindow().getSize().x);
	uniform_real_distribution<float> y_dist(0.0f, Engine::GetWindow().getSize().y);

	//Monkey Code
	for (size_t n = 0; n < 1; n++)
	{
		auto enemy = makeEntity();
		enemy->setPosition(Vector2f(x_dist(engine), y_dist(engine)));
		enemy->addTag("enemy");
		auto s = enemy->addComponent<SpriteComponent>();
		s->getSprite().setTexture(enemyTexture);
		s->getSprite().setTextureRect(enemyRect);
		s->getSprite().setOrigin(32.f, 32.f);
		enemy->addComponent<SteeringComponent>(player.get());
		enemy->addComponent<BasicAiMovementComponent>();
	};

	// Draw the UI overlay
	{
		// load the sprite
		Texture mainBanner;
		mainBanner.loadFromFile("resources/textures/SeaChainMainBanner.png", IntRect(0, 0, 1920, 1080));
		shared_ptr<Texture> spriteBanner = make_shared<Texture>(mainBanner);
		Texture healthBar;
		healthBar.loadFromFile("resources/textures/SeaChainHealthBar.png", IntRect(0, 0, 224, 33));
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

	// Draw/create the quit confirmation box with its two buttons
	{
		// load the sprite
		Texture box;
		box.loadFromFile("resources/textures/QuitMenu.png", IntRect(0, 0, 1920, 1080));
		shared_ptr<Texture> spriteBox = make_shared<Texture>(box);

		// set the position, add a tag, add the sprite component with the texture
		auto popup = makeEntity();
		popup->addTag("quitPopup");
		popup->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto popupSprite = popup->addComponent<SpriteComponent>();
		popupSprite->setTexure(spriteBox);
		popupSprite->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
		popup->setVisible(false);
		// draw/create the menu button
		{
			auto menuButton = makeEntity();
			menuButton->addTag("btnMenu");
			menuButton->setPosition(Vector2f(770, 565));
			auto buttonShape = menuButton->addComponent<ShapeComponent>();
			buttonShape->setShape<RectangleShape>(Vector2f(150, 43));
			buttonShape->getShape().setFillColor(Color::Transparent);
			buttonShape->getShape().setOutlineThickness(2);
			buttonShape->getShape().setOutlineColor(Color::White);
			menuButton->setVisible(debug);

			auto bounds = buttonShape->getBounds();

			btnMenu = menuButton->addComponent<ButtonComponent>();
			Vector2f xy = Vector2f(menuButton->getPosition().x + (bounds->width / 2), (menuButton->getPosition().y + (bounds->height / 2)));
			btnMenu->setBounds(xy, Vector2f(bounds->width, bounds->height));
			btnMenu->setInteraction(false);
		}
		// draw/create the quit button
		{
			auto exitButton = makeEntity();
			exitButton->addTag("btnQuit");
			exitButton->setPosition(Vector2f(940, 565));
			auto buttonShape = exitButton->addComponent<ShapeComponent>();
			buttonShape->setShape<RectangleShape>(Vector2f(150, 43));
			buttonShape->getShape().setFillColor(Color::Transparent);
			buttonShape->getShape().setOutlineThickness(2);
			buttonShape->getShape().setOutlineColor(Color::White);
			exitButton->setVisible(debug);

			auto bounds = buttonShape->getBounds();

			btnQuit = exitButton->addComponent<ButtonComponent>();
			Vector2f xy = Vector2f(exitButton->getPosition().x + (bounds->width / 2), (exitButton->getPosition().y + (bounds->height / 2)));
			btnQuit->setBounds(xy, Vector2f(bounds->width, bounds->height));
			btnQuit->setInteraction(false);
		}
	}

	

	/*auto path = pathFind(Vector2i((enemy->getPosition().x / 64), (enemy->getPosition().y / 64)), Vector2i(ls::getWidth() - 10, ls::getHeight() - 6));
	auto ai = enemy->addComponent<PathfindingComponent>();
	ai->setPath(path);*/
	

	//Simulate long loading times
	//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");

	setLoaded(true);
}

void TutorialMain::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
	player.reset();
	Nullify();
	ls::unload();
	Scene::UnLoad();
}

void TutorialMain::Update(const double& dt) {
	bool changingScenes = false;
	auto ins = Data::getInstance();
	auto keybinds = ins->getKeybinds();
	auto enemy_Path_Node_One = Vector2i(6, 4);
	auto enemy_Path_Node_Two = Vector2i(7, 11);

	if (length(player->getPosition() - enemy->getPosition()) < 50 && pause == false) {
		ins->setPlayer(player);
		changingScenes = true;
		enemy->setForDelete();
		Engine::ChangeScene(&combat);
	}

	if (!changingScenes)
	{
		if (pause == false) {
			// Movement annimation
			{
				if (ins->getBiscuit() == false) {
					auto s = player->GetCompatibleComponent<SpriteComponent>()[0];
					static sf::Clock clock;
					float elapsed = clock.getElapsedTime().asSeconds();

					//PLAYER ANIMATION FOR UP
					if (Keyboard::isKeyPressed(keybinds->find("MOVE_UP")->second))
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
					else if (Keyboard::isKeyPressed(keybinds->find("MOVE_DOWN")->second))
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
					else if (Keyboard::isKeyPressed(keybinds->find("MOVE_LEFT")->second))
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
					else if (Keyboard::isKeyPressed(keybinds->find("MOVE_RIGHT")->second))
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
				}
			}
			//For testing pathfinding on left click
			/*static bool mouse_down = false;
			if (Mouse::isButtonPressed(Mouse::Left) && !mouse_down)
			{
				auto mouse_pos = Mouse::getPosition(Engine::GetWindow());
				mouse_down = true;
				if (ls::isOnGrid(Vector2f(mouse_pos)))
				{
					auto relative_pos = mouse_pos - Vector2i(ls::getOffset());
					auto tile_coord = relative_pos / (int)ls::getTileSize();

					auto char_relative = enemy->getPosition() - ls::getOffset();
					auto char_tile = Vector2i(char_relative / ls::getTileSize());
					auto path = pathFind(char_tile, tile_coord);
					auto ai = enemy->GetCompatibleComponent<PathfindingComponent>()[0];
					ai->setPath(path);
				}
			}
			if (mouse_down && !Mouse::isButtonPressed(Mouse::Left))
			{
				mouse_down = false;
			}*/

			auto enemy_current_pos = Vector2i(enemy->getPosition().x / 64, (enemy->getPosition().y - ls::getOffset().y) / 64);
			cout << enemy_current_pos << endl;
			static bool test = true;
			//[6,4]// BOOL TRUE TEST BEFORE REMOVING BOOL Ensure enemy has pathfind component BEFORE move!
			if (enemy_current_pos == enemy_Path_Node_One )
			{
				auto path = pathFind(enemy_current_pos, enemy_Path_Node_Two);
				auto pathComp = enemy->GetCompatibleComponent<PathfindingComponent>()[0];
				pathComp->setPath(path);
				test = false;
			}
			//[7,11]
			else if (enemy_current_pos == enemy_Path_Node_Two )
			{
				auto path = pathFind(enemy_current_pos, enemy_Path_Node_One);
				auto pathComp = enemy->GetCompatibleComponent<PathfindingComponent>()[0];
				pathComp->setPath(path);
				test = true;
			}

			updateHealthBars(dt, changingScenes);
			Scene::Update(dt);
		}
		else {
			btnMenu->update(dt);
			btnQuit->update(dt);
		}
		checkEventPresses(dt, changingScenes);
	}
}

void TutorialMain::checkEventPresses(const double& dt, bool& changingScenes) {
	static sf::Clock bedCooldown;
	static float triggertime = 0.0f;
	triggertime -= dt;
	auto ins = Data::getInstance();
	auto keybinds = ins->getKeybinds();

	if (sf::Keyboard::isKeyPressed(keybinds->find("INTERACT")->second) && pause == false) {
		auto bed = this->ents.find("bed")[0];
		auto skeleton = this->ents.find("skeleton");

		// check the player is close to the bed
		if (length(player->getPosition() - bed->getPosition()) < ls::getTileSize())
		{
			// clock counter as a cooldown for using the bed, can only use it once ever 1 minute (its in seconds)
			if (bedCooldown.getElapsedTime().asSeconds() > 5)
			{
				// get player health component and set the health to max health.
				auto player = ins->getPlayer();
				ins->setBiscuit(false);
				auto health = player->GetCompatibleComponent<HealthComponent>()[0];
				health->setHealth(health->getMaxHealth());
				// reset the cooldown clock.
				bedCooldown.restart();
				changingScenes = true;
				Engine::ChangeScene(&tutorialMain);
			}
		}
		// loop through all skeleton entities on the map and check if player is within range
		if (!changingScenes) {
			for (auto s : skeleton) {
				if (length(player->getPosition() - s->getPosition()) < ls::getTileSize()) {
					// get inventory component, create the weapon and add it to the inventory
					std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];
					auto wep = Weapon("sword", Item::Quality::Iron, 5, 50, 100);
					ic->addWeapon(wep);
					ic->setUsing(0);
					// delete the skeleton as it's been looted
					s->setForDelete();
				}
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(keybinds->find("GO_BACK")->second) && triggertime < 0)
	{
		auto popup = this->ents.find("quitPopup")[0];
		// if the popup is already displayed.
		if (popup->isVisible()) {
			popup->setVisible(false);
			btnQuit->setInteraction(false);
			btnMenu->setInteraction(false);
			pause = false;
		}
		else
		{
			popup->setVisible(true);
			btnQuit->setInteraction(true);
			btnMenu->setInteraction(true);
			pause = true;
		}

		triggertime = 0.8f;
	}
	if (changingScenes == false) {
		if (btnMenu->isPressed()) {
			changingScenes = true;
			Engine::ChangeScene(&menu);
		}
		else if (btnQuit->isPressed()) {
			changingScenes = true;
			Nullify();
			Engine::GetWindow().close();
			std::exit(EXIT_SUCCESS);
		}
	}
}

void TutorialMain::updateHealthBars(const double& dt, bool changingScenes) {
	if (!changingScenes) {
		auto ins = Data::getInstance();
		auto player = ins->getPlayer();
		auto playerHealth = player->GetCompatibleComponent<HealthComponent>()[0];
		float barWidth;

		auto uiBar = this->ents.find("healthUIBar")[0];
		auto spriteComponent = uiBar->GetCompatibleComponent<SpriteComponent>()[0];
		barWidth = max((playerHealth->getHealth() / playerHealth->getMaxHealth()) * spriteComponent->getSprite().getTexture()->getSize().x, 0.f);
		spriteComponent->getSprite().setTextureRect(IntRect(0, 0, static_cast<int>(barWidth), spriteComponent->getBounds()->height));
	}
}

void TutorialMain::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void TutorialMain::createTexture(std::string path, sf::IntRect bounds, std::vector<sf::Vector2ul> tiles, std::string tag)
{
	//Tile Texture
	Texture worldSpriteSheet;
	worldSpriteSheet.loadFromFile(path, bounds);

	shared_ptr<Texture> worldSheet = make_shared<Texture>(worldSpriteSheet);
	for (auto t : tiles) {
		auto pos = ls::getTilePosition(t);
		auto e = makeEntity();
		e->setPosition(pos);
		e->addTag(tag);
		auto t = e->addComponent<SpriteComponent>();
		t->setTexure(worldSheet);
	}
}

void TutorialMain::createWaterBlend() {
	Texture edgeDown;
	Texture edgeLeft;
	Texture edgeRight;
	Texture edgeUp;
	edgeDown.loadFromFile("resources/textures/WaterEdgeDOWN.png", IntRect(0, 0, 64, 64));
	edgeLeft.loadFromFile("resources/textures/WaterEdgeLEFT.png", IntRect(0, 0, 64, 64));
	edgeRight.loadFromFile("resources/textures/WaterEdgeRIGHT.png", IntRect(0, 0, 64, 64));
	edgeUp.loadFromFile("resources/textures/WaterEdgeUP.png", IntRect(0, 0, 64, 64));
	shared_ptr<Texture> waterDown = make_shared<Texture>(edgeDown);
	shared_ptr<Texture> waterLeft = make_shared<Texture>(edgeLeft);
	shared_ptr<Texture> waterRight = make_shared<Texture>(edgeRight);
	shared_ptr<Texture> waterUp = make_shared<Texture>(edgeUp);
	auto windowSize = Engine::getWindowSize();

	auto waterTiles = ls::findTiles(ls::WATER);
	// loop through all possible water tiles.
	for (auto w : waterTiles) {
		// avoid word 'else' as we have to check multiple directions of some tiles.

		// check below the tile isn't water
		// prepare for any index out of range and integer overflow
		if ((w.y + 1) >= 0 && (w.y + 1) <= ls::getHeight() && ls::getTile(Vector2ul(w.x, w.y + 1)) != ls::WATER) {
			// if it's not water, make a entity and assign the waterUp texture.
			auto banner = makeEntity();
			banner->addTag("waterUP");
			banner->setPosition(ls::getTilePosition(Vector2ul(w.x, w.y + 1)));
			auto bannerSprite = banner->addComponent<SpriteComponent>();
			bannerSprite->setTexure(waterUp);
		}
		// check above the tile isn't water
		// prepare for index out of range and integer overflow
		if ((w.y - 1) >= 0 && (w.y - 1) <= ls::getHeight() && ls::getTile(Vector2ul(w.x, w.y - 1)) != ls::WATER) {
			// if it's not water, make a entity and assign the waterDown texture.
			auto banner = makeEntity();
			banner->addTag("waterDOWN");
			banner->setPosition(ls::getTilePosition(Vector2ul(w.x, w.y - 1)));
			auto bannerSprite = banner->addComponent<SpriteComponent>();
			bannerSprite->setTexure(waterDown);
		}
		// check left of the tile isn't water
		// prepare for index out of range and integer overflow
		if ((w.x - 1) >= 0 && (w.x - 1) <= ls::getWidth() && ls::getTile(Vector2ul(w.x - 1, w.y)) != ls::WATER) {
			// if it's not water, make a entity and assign the waterRight texture.
			auto banner = makeEntity();
			banner->addTag("waterRIGHT");
			banner->setPosition(ls::getTilePosition(Vector2ul(w.x - 1, w.y)));
			auto bannerSprite = banner->addComponent<SpriteComponent>();
			bannerSprite->setTexure(waterRight);
		}
		// check right of the tile isn't water
		// prepare for index out of range and integer overflow
		if ((w.x + 1) >= 0 && (w.x + 1) <= ls::getWidth() && ls::getTile(Vector2ul(w.x + 1, w.y)) != ls::WATER) {
			// if it's not water, make a entity and assign the WaterLeft texture.
			auto banner = makeEntity();
			banner->addTag("waterLEFT");
			banner->setPosition(ls::getTilePosition(Vector2ul(w.x + 1, w.y)));
			auto bannerSprite = banner->addComponent<SpriteComponent>();
			bannerSprite->setTexure(waterLeft);
		}
 	}
}

int TutorialMain::randomNumber(int min, int max) {
	// Seed the random_device with the random engine and get a uniform_real_distribution between the min and max value
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<int> damage(min, max);

	return damage(engine);
}

void TutorialMain::Nullify()
{
	btnMenu = nullptr;
	btnQuit = nullptr;
}
