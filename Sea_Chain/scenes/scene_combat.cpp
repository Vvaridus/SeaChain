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
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;
static shared_ptr<Entity> playerMain;
static shared_ptr<Entity> enemy;
static bool playerTurn = false;
static bool dead = false;
static bool parry = false;
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

static sf::SoundBuffer sfxQuickAttack;
static sf::SoundBuffer sfxNormalAttack;
static sf::SoundBuffer sfxHeavyAttack;
static sf::SoundBuffer sfxParryAttack;
static sf::Sound sound;
static float volume;
static sf::Clock timer;

void CombatScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

	playerTurn = true;
	dead = false;

	sfxQuickAttack.loadFromFile("resources/sound/SeaChainQuickSlash.wav");
	sfxNormalAttack.loadFromFile("resources/sound/SeaChainNormalAttack.wav");
	sfxHeavyAttack.loadFromFile("resources/sound/SeaChainHeavyAttack.wav");
	sfxParryAttack.loadFromFile("resources/sound/SeaChainParry.wav");

	auto ins = Data::getInstance();
	volume = ins->getSoundVolume();

	auto windowSize = Engine::getWindowSize();

	// Draw background overlay
	{
		// load the sprite
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/SeaChainCombatOverlayANDBG.png", IntRect(0, 0, 1920, 1080));
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
		spritesheet.loadFromFile("resources/textures/SeaChainCombatOverlay.png", IntRect(0, 0, 1920, 1080));
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

	// Draw the enemy
	{
		// We declare enemy after everything else to make sure they are on top
		// make the enemy entity with a healt and attack component
		enemy = makeEntity();
		enemy->addComponent<HealthComponent>();
		enemy->addComponent<EnemyAttackComponent>();

		// load the sprite
		Texture enemySpriteSheet;
		enemySpriteSheet.loadFromFile("resources/textures/SeaChainEnemy.png", IntRect(0, 64, 64, 64));
		shared_ptr<Texture> spriteCharacter = make_shared<Texture>(enemySpriteSheet);

		Texture healthBarOverlaySpriteSheet;
		healthBarOverlaySpriteSheet.loadFromFile("resources/textures/HealthBarRed.png", IntRect(0, 0, 256, 64));
		shared_ptr<Texture> spriteHealthBarOverlay = make_shared<Texture>(healthBarOverlaySpriteSheet);

		Texture healthbarSpriteSheet;
		healthbarSpriteSheet.loadFromFile("resources/textures/HealthBarOutline.png", IntRect(0, 0, 256, 64));
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
	}

	// Draw the player
	{
		// This entity is soley for drawing, since the one in Data belongs to the scene, Tutorial_main
		// We declare player after everything else to make sure they are on top
		playerMain = makeEntity();
		// load the sprite
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/SeaChainPlayer.png", IntRect(0, 128, 64, 64));
		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		// set the position, add a tag, add the sprite component with the texture
		playerMain->setPosition(Vector2f(0.65 * windowSize.x, windowSize.y / 2));
		auto spriteComp = playerMain->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(sprite->getSize().x / 2, sprite->getSize().y / 2));
		spriteComp->setScaling(Vector2f(4, 4));
	}

	// Draw text log of whats happening
	{
		auto text = makeEntity();
		text->addTag("textLog");
		text->setPosition(Vector2f(0.38 * windowSize.x, 0.78 * windowSize.y));
		//auto textShape = text->addComponent<ShapeComponent>();
		//textShape->setShape<CircleShape>(15.f);
		//textShape->getShape().setFillColor(Color::White);
		//textShape->getShape().setOrigin(Vector2f(7.5, 7.5));
		auto textBox = text->addComponent<TextComponent>("Let the battle \nbegin!");
		textBox->setFillColor(Color::White);
		textBox->setCharSize(32);
		textBox->setPosition(text->getPosition());
		textBox->setOrigin(Vector2f(textBox->getBounds().width / 2, textBox->getBounds().height / 2));
	}

	// Error for entering without weapon
	{
		auto text = makeEntity();
		text->addTag("errorText");
		text->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		//auto textShape = text->addComponent<ShapeComponent>();
		//textShape->setShape<CircleShape>(15.f);
		//textShape->getShape().setFillColor(Color::White);
		//textShape->getShape().setOrigin(Vector2f(7.5, 7.5));
		auto textBox = text->addComponent<TextComponent>("What you going to fight with? Huh? \n\t\tHopes and dreams?");
		textBox->setFillColor(Color::Red);
		textBox->setCharSize(64);
		textBox->setPosition(text->getPosition());
		textBox->setOrigin(Vector2f(textBox->getBounds().width / 2, textBox->getBounds().height / 2));
		text->setVisible(false);
	}


	cout << enemy->getPosition() << "\n" << playerMain->getPosition() << endl;

	createButtons();

	//Simulate long loading times
	//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");
	timer.restart();

	setLoaded(true);
}

void CombatScene::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
	nullify();
	Scene::UnLoad();
}

void CombatScene::Update(const double& dt) {
	Weapon wep;
	bool changingScene = false;
	AttackData at;
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto enemyAttack = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];
	std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];

	// if enemy or user dead return to main island
	if (dead) {
		changingScene = true;
		Engine::ChangeScene(&tutorialMain);

	}
	// if user has no weapon go back to main island
	if (ic->getUsing() < 0) {
		auto text = this->ents.find("errorText")[0];
		text->setVisible(true);

		if (timer.getElapsedTime().asSeconds() > 6) {
			// Easter Egg?!?!?!
			{
				int chance = randomNumber(0, 100);
				if (chance <= 25)
					// Biscuit mode!
					ins->setBiscuit(true);
			}

			changingScene = true;
			Engine::ChangeScene(&tutorialMain);
		}
		else if (timer.getElapsedTime().asSeconds() > 3) {
			auto txt = text->GetCompatibleComponent<TextComponent>()[0];
			txt->SetText("Scatter! ya biscuit eater!");
			txt->setOrigin(Vector2f(txt->getBounds().width / 2, txt->getBounds().height / 2));
		}
	}

	if (btnRun->isPressed())
		escape(dt, changingScene);

	if (!changingScene)
	{
		if (btnQuickAttack->isPressed()) {
			//play attack sound
			sound.setBuffer(sfxQuickAttack);
			sound.setVolume(volume);
			sound.play();
			// Get the attack stats with the quick move
			parry = false;
			at = getAttackStats(attackType::Quick, "player");
			// Attack the enemy with the attack stats
			attack(at, "enemy");

		}
		else if (btnNormalAttack->isPressed()) {
			//play attack sound
			sound.setBuffer(sfxNormalAttack);
			sound.setVolume(volume);
			sound.play();
			// Get the attack stats with the quick move
			parry = false;
			at = getAttackStats(attackType::Normal, "player");
			// Attack the enemy with the attack stats
			attack(at, "enemy");
		}
		else if (btnHeavyAttack->isPressed()) {
			//play attack sound
			sound.setBuffer(sfxHeavyAttack);
			sound.setVolume(volume);
			sound.play();
			// Get the attack stats with the quick move
			parry = false;
			at = getAttackStats(attackType::Heavy, "player");
			// Attack the enemy with the attack stats
			attack(at, "enemy");
		}
		else if (btnParry->isPressed()) {
			//play attack sound
			sound.setBuffer(sfxParryAttack);
			sound.setVolume(volume);
			sound.play();
			// Get the attack stats with the quick move
			parry = false;
			at = getAttackStats(attackType::Parry, "player");
			// Attack the enemy with the attack stats
			attack(at, "enemy");
		}
		// handle enemy turn.
		if (!playerTurn) {
			// get the attack stats and decide the move to make
			parry = false;
			at = getAttackStats(attackType::None, "enemy");
			// Attack the player with the attack stats
			attack(at, "player");
		}


		updateHealthBars(dt);
		Scene::Update(dt);
	}
}

AttackData CombatScene::getAttackStats(attackType type, std::string attacker) {
	int damage = 0;
	bool critSuccess = false;

	if (attacker == "player") {
		// find the weapon inside the player inventory component.
		auto ins = Data::getInstance();
		auto player = ins->getPlayer();
		std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];
		Weapon& wep = ic->findWeapon(ic->getUsing());
		// get the damage and crit of the weapon
		damage = wep.getDamage();
	}
	else if (attacker == "enemy") {
		// get the damage and crit from the EnemyAttackComponent.
		std::shared_ptr<EnemyAttackComponent> eac = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];
		type = eac->decideAttack();
		damage = eac->getDamage();
	}

	switch (type) {
	case attackType::Quick:
		if (randomNumber(0, 100) <= 5) {
			damage = damage * 2;
			critSuccess = true;
		}
		else
			critSuccess = false;
		break;
	case attackType::Normal:
		if (randomNumber(0, 100) <= 20) {
			damage = damage * 2;
			critSuccess = true;
		}
		else
			critSuccess = false;
		break;
	case attackType::Heavy:
		if (randomNumber(0, 100) <= 75) {
			damage = damage * 2;
			critSuccess = true;
		}
		else
			critSuccess = false;
		break;
	case attackType::Parry:
		parry = true;
		damage = 0;
		break;
	}


	// Return the attack data gathered
	return AttackData(type, damage, parry, critSuccess);
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
		enemyAttack->setHumanAttack(ad.attack);
		updateLog("Player", "Enemy", ad);
	}
	// Enemy AI attacking the Player
	else if (beingAttacked == "player") {
		playerHealth->setHealth(playerHealth->getHealth() - ad.damage);
		enemyAttack->setEnemyAttack(ad.attack);
		updateLog("Enemy", "Player", ad);
	}

	enemyAttack->setHumanHealth(player->GetCompatibleComponent<HealthComponent>()[0]->getHealth());
	enemyAttack->setHumanMaxHealth(player->GetCompatibleComponent<HealthComponent>()[0]->getMaxHealth());

	if (ad.attack == attackType::Parry)
	{
		// if move if parry, 3% chance you will retailate with quick attack and make enemy miss a turn.
		int chance = randomNumber(0, 100);
		if (chance <= 3) {
			attack(getAttackStats(attackType::Quick, "player"), "enemy");
			parry = true;
		}
		cout << chance << endl;
	}

	if (!parry)
		playerTurn = !playerTurn;

}

void CombatScene::updateLog(string attacking, string defending, AttackData ad) {
	auto textBox = this->ents.find("textLog")[0];
	auto text = textBox->GetCompatibleComponent<TextComponent>()[0];
	string move;

	switch (ad.attack) {
	case attackType::Quick: move = "Quick Attack"; break;
	case attackType::Normal: move = "Normal Attack"; break;
	case attackType::Heavy: move = "Heavy Attack"; break;
	case attackType::Parry: move = "Parry Attack"; break;
	case attackType::None: move = "Uh oh!"; break;
	};

	string output = attacking + " Has chosen \n" + move + ".";
	if (ad.attack != attackType::Parry)
		output += "\nWhich dealt " + to_string(ad.damage) + "\nto " + defending;
	if (ad.critSuccess == true)
		output += " \nwith a successful \ncrit!";

	text->SetText(output);
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
	if (barWidth <= 0)
		dead = true;

	auto enemyHealthBar = this->ents.find("enemyHealthBar")[0];
	spriteComponent = enemyHealthBar->GetCompatibleComponent<SpriteComponent>()[0];
	barWidth = max((enemyHealth->getHealth() / enemyHealth->getMaxHealth()) * spriteComponent->getSprite().getTexture()->getSize().x, 0.f);
	spriteComponent->getSprite().setTextureRect(IntRect(0, 0, static_cast<int>(barWidth), spriteComponent->getBounds()->height));
	if (barWidth <= 0)
		dead = true;
}

void CombatScene::escape(const double& dt, bool &changingScene) {
	int chance = randomNumber(0, 100);
	static sf::Clock timer;
	static bool run;
	if (chance <= 25) {
		auto text = this->ents.find("errorText")[0];
		text->setVisible(true);
		auto txt = text->GetCompatibleComponent<TextComponent>()[0];
		txt->SetText("Scatter! ya biscuit eater!");
		txt->setOrigin(Vector2f(txt->getBounds().width / 2, txt->getBounds().height / 2));
		run = true;
		timer.restart();
	}

	if (run && timer.getElapsedTime().asSeconds() > 5) {
		Engine::ChangeScene(&tutorialMain);
		changingScene = true;
	}
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