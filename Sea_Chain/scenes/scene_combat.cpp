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
#include <system_renderer.h>
#include "../components/cmp_button.h"
#include "../components/cmp_text.h"
#include "../components/cmp_health.h"
#include "../components/cmp_enemy_attack.h"
#include <random>

using namespace std;
using namespace sf;
static shared_ptr<Entity> playerMain;
static shared_ptr<Entity> enemy;
static bool playerTurn = false;
static shared_ptr<ButtonComponent> btnBribe;
static shared_ptr<ButtonComponent> btnRun;
static shared_ptr<ButtonComponent> btnWepSwap;
static shared_ptr<ButtonComponent> btnConsum;
static shared_ptr<ButtonComponent> btnQuickAttack;
static shared_ptr<ButtonComponent> btnNormalAttack;
static shared_ptr<ButtonComponent> btnHeavyAttack;
static shared_ptr<ButtonComponent> btnParry;
static enum class attackType {
	Quick,
	Normal,
	Heavy,
	Parry
};

void CombatScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

	playerTurn = true;
	
	auto windowSize = Engine::getWindowSize();

	// Draw background overlay
	{
		// load the sprite
		Texture spritesheet;
		spritesheet.loadFromFile("resources/SeaChainCombatOverlayANDBG.png", IntRect(0, 0, 1920, 1080));
		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);
	
		// set the position, add a tag, add the sprite component with the texture
		auto background = makeEntity();
		background->addTag("background");
		background->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = background->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}
	
	// Draw the combat overlay
	{
		// load the sprite
		Texture spritesheet;
		spritesheet.loadFromFile("resources/SeaChainCombatOverlay.png", IntRect(0, 0, 1920, 1080));
		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);
	
		// set the position, add a tag, add the sprite component with the texture
		auto combat = makeEntity();
		combat->addTag("combatOverlayImage");
		combat->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = combat->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
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
		healthBanner->setPosition(Vector2f(256, 34));
		auto healthSprite = healthBanner->addComponent<SpriteComponent>();
		healthSprite->setTexure(spriteHealth);
	}

	// Draw the enemy
	{
		// We declare enemy after everything else to make sure they are on top
		// make the enemy entity with a healt and attack component
		enemy = makeEntity();
		enemy->addComponent<HealthComponent>();
		enemy->addComponent<EnemyAttackComponent>();

		// load the sprite
		Texture enemySpriteSheet;
		enemySpriteSheet.loadFromFile("resources/SeaChainEnemy.png", IntRect(0, 64, 64, 64));
		shared_ptr<Texture> spriteCharacter = make_shared<Texture>(enemySpriteSheet);

		Texture healthBarOverlaySpriteSheet;
		healthBarOverlaySpriteSheet.loadFromFile("resources/HealthBarRed.png", IntRect(0, 0, 256, 64));
		shared_ptr<Texture> spriteHealthBarOverlay = make_shared<Texture>(healthBarOverlaySpriteSheet);

		Texture healthbarSpriteSheet;
		healthbarSpriteSheet.loadFromFile("resources/HealthBarOutline.png", IntRect(0, 0, 256, 64));
		shared_ptr<Texture> spriteHealthBarBackground = make_shared<Texture>(healthbarSpriteSheet);

		// set the position, add a tag, add the sprite component with the texture
		enemy->setPosition(Vector2f(0.35 * windowSize.x, windowSize.y / 2));
		auto spriteComp = enemy->addComponent<SpriteComponent>();
		spriteComp->setTexure(spriteCharacter);
		spriteComp->setOrigin(Vector2f(spriteCharacter->getSize().x / 2, spriteCharacter->getSize().y / 2));
		spriteComp->setScaling(Vector2f(4, 4));

		// Create another healthbar entity so we can set the position seperately.
		// Add two different spriteComponents, the background and overlay. 
		auto healthBar = makeEntity();
		healthBar->setPosition(Vector2f(0.35 * windowSize.x, 0.35 * windowSize.y));

		healthBar->addTag("enemyHealthBar");
		auto healthBarSprite = healthBar->addComponent<SpriteComponent>();
		healthBarSprite->setTexure(spriteHealthBarOverlay);
		healthBarSprite->setOrigin(Vector2f(spriteHealthBarOverlay->getSize().x / 2, spriteHealthBarOverlay->getSize().y / 2));

		auto healthBarBackgroundSprite = healthBar->addComponent<SpriteComponent>();
		healthBarBackgroundSprite->setTexure(spriteHealthBarBackground);
		healthBarBackgroundSprite->setOrigin(Vector2f(spriteHealthBarBackground->getSize().x / 2, spriteHealthBarBackground->getSize().y / 2));
		//healthBarBackgroundSprite->setScaling(Vector2f(0.95, 0.8));
	}

	// Draw the player
	{
		// This entity is soley for drawing, since the one in Data belongs to the scene, Tutorial_main
		// We declare player after everything else to make sure they are on top
		playerMain = makeEntity();
		// load the sprite
		Texture spritesheet;
		spritesheet.loadFromFile("resources/SeaChainPlayer.png", IntRect(0, 128, 64, 64));
		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);
	
		// set the position, add a tag, add the sprite component with the texture
		playerMain->setPosition(Vector2f(0.65 * windowSize.x, windowSize.y / 2));
		auto spriteComp = playerMain->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(sprite->getSize().x / 2, sprite->getSize().y / 2));
		spriteComp->setScaling(Vector2f(4, 4));
	}

	cout << enemy->getPosition() << "\n" << playerMain->getPosition() << endl;

	createButtons();

	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");

	setLoaded(true);
}

void CombatScene::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
	nullify();
	Scene::UnLoad();
}

void CombatScene::Update(const double& dt) {
	Weapon wep;
	AttackData at;
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto enemyAttack = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		// Go back to previous screen, nullify and change scene
		Engine::ChangeScene(&tutorialMain);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		// triggertime for a cool down on pressing the key, so it doesn't trigger several times in a second.
		static float triggertime = 0.0f;
		triggertime -= dt;
		if (triggertime <= 0)
		{
			// get the inventory, create the weapon, add it and set it to using.
			std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];
			wep = Weapon("sword", Item::Quality::Iron, 5, 50, 100, 50, 20);
			ic->addWeapon(wep);
			ic->setUsing(0);

			cout << "Added item" << endl;
			// set the cooldown
			triggertime = .5f;
		}
	}
	else if (btnQuickAttack->isPressed()) {
		// Get the attack stats with the quick move
		at = getAttackStats(attackType::Quick, "player");
		// Attack the enemy with the attack stats
		attack(at, "enemy");
	}
	// handle enemy turn.
	if (!playerTurn) {
		// get the attack stats and decide the move to make
		at = getAttackStats(attackType::None, "enemy");
		// Attack the player with the attack stats
		attack(at, "player");
	}

	updateHealthBars(dt);
	Scene::Update(dt);
}

AttackData CombatScene::getAttackStats(attackType type, std::string attacker) {
	int damage, crit = 0;
	bool critSuccess;

	if (attacker == "player") {
		// find the weapon inside the player inventory component.
		auto ins = Data::getInstance();
		auto player = ins->getPlayer();
		std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];
		Weapon& wep = ic->findWeapon(ic->getUsing());
		// get the damage and crit of the weapon
		damage = wep.getDamage();
		crit = wep.getCrit();
	}
	else if (attacker == "enemy") {
		// get the damage and crit from the EnemyAttackComponent.
		std::shared_ptr<EnemyAttackComponent> eac = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];
		type = eac->decideAttack();
		damage = eac->getDamage();
		crit = eac->getCritChance();
	}
	// generate a random number to check if crit will succeed. if so double the damage
	if (randomNumber(0, 100) <= crit) {
		damage = damage * 2;
		critSuccess = true;
	}
	else
		critSuccess = false;

	// Return the attack data gathered
	return AttackData(type, damage, 0, 0, critSuccess);
}

void CombatScene::attack(AttackData ad, std::string beingAttacked) {
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto playerHealth = player->GetCompatibleComponent<HealthComponent>()[0];
	auto enemyAttack = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];

	// Player attacking the enemy AI
	if (beingAttacked == "enemy") {
		auto enemyHealth = enemy->GetCompatibleComponent<HealthComponent>()[0];
		enemyHealth->setHealth(enemyHealth->getHealth() - ad.damage);
		cout << "PLAYER ATTACKING ENEMY: " << enemyHealth->getHealth() << " : " << ad.damage << " : " << ad.critChance << endl;
		enemyAttack->setHumanAttack(ad.attack);
	}
	// Enemy AI attacking the Player
	else if (beingAttacked == "player") {
		playerHealth->setHealth(playerHealth->getHealth() - ad.damage);
		cout << "ENEMY ATTACKING PLAYER: " << playerHealth->getHealth() << " : " << ad.damage << " : " << ad.critChance << endl;
		enemyAttack->setEnemyAttack(ad.attack);
	}

	enemyAttack->setHumanHealth(player->GetCompatibleComponent<HealthComponent>()[0]->getHealth());
	enemyAttack->setHumanMaxHealth(player->GetCompatibleComponent<HealthComponent>()[0]->getMaxHealth());
	playerTurn = !playerTurn;
}

void CombatScene::updateHealthBars(const double& dt) {
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto enemyHealth = enemy->GetCompatibleComponent<HealthComponent>()[0];
	auto playerHealth = player->GetCompatibleComponent<HealthComponent>()[0];
	float barWidth;

	auto uiBar = this->ents.find("healthUIBar")[0];
	auto spriteComponent = uiBar->GetCompatibleComponent<SpriteComponent>()[0];
	barWidth = max((playerHealth->getHealth() / playerHealth->getMaxHealth()) * spriteComponent->getSprite().getTexture()->getSize().x, 0.f);
	spriteComponent->getSprite().setTextureRect(IntRect(0, 0, static_cast<int>(barWidth), spriteComponent->getBounds()->height));

	auto enemyHealthBar = this->ents.find("enemyHealthBar")[0];
	spriteComponent = enemyHealthBar->GetCompatibleComponent<SpriteComponent>()[0];
	barWidth = max((enemyHealth->getHealth() / enemyHealth->getMaxHealth()) * spriteComponent->getSprite().getTexture()->getSize().x, 0.f);
	spriteComponent->getSprite().setTextureRect(IntRect(0, 0, static_cast<int>(barWidth), spriteComponent->getBounds()->height));
}

void CombatScene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void CombatScene::createButtons() {
	Vector2f choiceBtnDimentions = Vector2f(128, 128);
	Vector2f attackBtnDimentions = Vector2f(150, 150);
	auto ins = Data::getInstance();
	bool debug = ins->getDebug();

	//Draw first button (BRIBE BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnBribe");
		button->setPosition(Vector2f(97, 780));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(choiceBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnBribe = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnBribe->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw second button (RUN BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnRun");
		button->setPosition(Vector2f(245, 780));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(choiceBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnRun = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnRun->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw third button (SWAP WEAPON BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnWepSwap");
		button->setPosition(Vector2f(97, 929));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(choiceBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnWepSwap = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnWepSwap->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw fourth button (CONSUMABLE BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnConsum");
		button->setPosition(Vector2f(245, 929));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(choiceBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnConsum = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnConsum->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw fifth button (QUICK ATTACK BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnQuickAttack");
		button->setPosition(Vector2f(1166, 842));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(attackBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnQuickAttack = button->addComponent<ButtonComponent>();
		btnQuickAttack = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnQuickAttack->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw sixth button (NORMAL ATTACK BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnNormalAttack");
		button->setPosition(Vector2f(1336, 842));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(attackBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnNormalAttack = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnNormalAttack->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw seventh button (NORMAL ATTACK BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnHeavyAttack");
		button->setPosition(Vector2f(1506, 842));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(attackBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnHeavyAttack = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnHeavyAttack->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//Draw eighth button (PARRY BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnParry");
		button->setPosition(Vector2f(1676, 842));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(attackBtnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();
		btnParry = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnParry->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
}

void CombatScene::nullify() {
	// Nullify all the buttons, otherwise the entity can't be deleted

	btnBribe = nullptr;
	btnRun = nullptr;
	btnWepSwap = nullptr;
	btnConsum = nullptr;
	btnQuickAttack = nullptr;
	btnNormalAttack = nullptr;
	btnHeavyAttack = nullptr;
	btnParry = nullptr;
}

int CombatScene::randomNumber(int min, int max) {
	// Seed the random_device with the random engine and get a uniform_real_distribution between the min and max value
	std::random_device dev;
	std::default_random_engine engine(dev());
	std::uniform_int_distribution<int> damage(min, max);

	return damage(engine);
}