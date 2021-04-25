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

using namespace std;
using namespace sf;
static shared_ptr<Entity> player;
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
	playerMain = makeEntity();
	enemy = makeEntity();
	enemy->addComponent<HealthComponent>();
	enemy->addComponent<EnemyAttackComponent>();

	auto windowSize = Engine::getWindowSize();

	// Draw temp background colour
	{
		auto combat = makeEntity();
		combat->addTag("background");
		combat->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto backgroundRect = combat->addComponent<ShapeComponent>();
		backgroundRect->setShape<sf::RectangleShape>(Vector2f(1920, 1080));
		backgroundRect->getShape().setFillColor(Color::White);
		backgroundRect->getShape().setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}

	// Draw the combat overlay
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/SeaChainCombatOverlay.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);


		auto combat = makeEntity();
		combat->addTag("combatOverlayImage");
		combat->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = combat->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}

	// Draw the UI overlay
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/SeaChainMainBanner.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		auto combat = makeEntity();
		combat->addTag("mainBanner");
		combat->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = combat->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}

	createButtons();

	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");

	setLoaded(true);
}

void CombatScene::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
	ls::unload();
	Scene::UnLoad();
}

void CombatScene::Update(const double& dt) {
	Weapon wep;
	AttackData at;
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto enemyAttack = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
		nullify();
		Engine::ChangeScene(&tutorialMain);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		static float triggertime = 0.0f;
		triggertime -= dt;
		if (triggertime <= 0)
		{
			std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];
			//std::string itemID, int id, Item::Quality quality, int minDamage, int maxDamage, int uses
			wep = Weapon("sword", Item::Quality::Iron, 5, 50, 100, 50, 20);
			ic->addWeapon(wep);
			ic->setUsing(0);

			cout << "Added item" << endl;
			triggertime = .5f;
		}
	}
	else if (btnQuickAttack->isPressed()) {
		at = handlePlayerAttack(attackType::Quick);

		attack(at, "enemy");
	}

	// handle enemy turn.
	if (!playerTurn) {
		auto attackMove = enemyAttack->decideAttack();
		attack(AttackData(CombatScene::attackType::Quick, enemyAttack->getDamage(), 0, 0, 0), "player");
	}
	Scene::Update(dt);
}

void CombatScene::attack(AttackData ad, std::string attacked)
{
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	auto playerHealth = player->GetCompatibleComponent<HealthComponent>()[0];
	auto enemyHealth = enemy->GetCompatibleComponent<HealthComponent>()[0];
	auto enemyAttack = enemy->GetCompatibleComponent<EnemyAttackComponent>()[0];

	if (attacked == "enemy") {
		enemyHealth->setHealth(enemyHealth->getHealth() - ad.damage);

		cout << "PLAYER ATTACKING ENEMY: " << enemyHealth->getHealth() << " : " << ad.damage << " : " << ad.critChance << endl;

		enemyAttack->setHumanAttack(ad.attack);

		playerTurn = !playerTurn;
	}
	if (attacked == "player") {
		playerHealth->setHealth(playerHealth->getHealth() - ad.damage);

		cout << "ENEMY ATTACKING PLAYER: " << playerHealth->getHealth() << " : " << ad.damage << " : " << ad.critChance << endl;

		enemyAttack->setEnemyAttack(ad.attack);
		playerTurn = !playerTurn;
	}

	enemyAttack->setHumanHealth(player->GetCompatibleComponent<HealthComponent>()[0]->getHealth());
	enemyAttack->setHumanMaxHealth(player->GetCompatibleComponent<HealthComponent>()[0]->getMaxHealth());
}

AttackData CombatScene::handlePlayerAttack(attackType attack) {
	AttackData at;
	switch (attack) {
	case attackType::Quick:
		at = quickAttack();
		break;
	}


	return at;
}

AttackData CombatScene::quickAttack()
{
	auto ins = Data::getInstance();
	auto player = ins->getPlayer();
	bool parrySuccess;
	bool critSuccess = false;

	// get the current weapon and the stats
	std::shared_ptr<InventoryComponent> ic = player->GetCompatibleComponent<InventoryComponent>()[0];
	Weapon& wep = ic->findWeapon(ic->getUsing());

	auto damage = wep.getDamage();
	auto crit = wep.getCrit();

	// find chance of crit and double damage if successful.
	auto random = rand() % 100 + 1; // 1-100
	if (random <= crit)
	{
		damage = damage * 2;
		critSuccess = true;
	}

	return AttackData(attackType::Quick, damage, 0, 0, critSuccess);
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
		btnBribe->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		btnRun->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		btnWepSwap->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		btnConsum->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnQuickAttack->setBounds(xy, Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		btnNormalAttack->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		btnHeavyAttack->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		btnParry->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
	}
}

void CombatScene::nullify() {
	btnBribe = nullptr;
	btnRun = nullptr;
	btnWepSwap = nullptr;
	btnConsum = nullptr;
	btnQuickAttack = nullptr;
	btnNormalAttack = nullptr;
	btnHeavyAttack = nullptr;
	btnParry = nullptr;
}
