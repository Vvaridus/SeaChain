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

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<ButtonComponent> btnBribe;
static shared_ptr<ButtonComponent> btnRun;
static shared_ptr<ButtonComponent> btnWepSwap;
static shared_ptr<ButtonComponent> btnConsum;
static shared_ptr<ButtonComponent> btnQuickAttack;
static shared_ptr<ButtonComponent> btnNormalAttack;
static shared_ptr<ButtonComponent> btnHeavyAttack;
static shared_ptr<ButtonComponent> btnParry;

void CombatScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

	auto ins = Data::getInstance();
	auto p = ins->getPlayer();
	player = makeEntity();
	//player = p->get_components();

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

	CreateButtons();

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
	Scene::Update(dt);
}

void CombatScene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}

void CombatScene::CreateButtons() {
	Vector2f choiceBtnDimentions = Vector2f(128, 128);
	Vector2f attackBtnDimentions = Vector2f(150, 150);
	bool drawOutline = false;

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
		button->setVisible(drawOutline);

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
		button->setVisible(drawOutline);

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
		button->setVisible(drawOutline);

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
		button->setVisible(drawOutline);

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
		button->setVisible(drawOutline);

		auto bounds = buttonShape->getBounds();

		btnQuickAttack = button->addComponent<ButtonComponent>();
		btnQuickAttack->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
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
		button->setVisible(drawOutline);

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
		button->setVisible(drawOutline);

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
		button->setVisible(drawOutline);

		auto bounds = buttonShape->getBounds();

		btnParry = button->addComponent<ButtonComponent>();
		btnParry->setBounds(Vector2f(button->getPosition().x, button->getPosition().y), Vector2f(bounds->width, bounds->height)); // this doesn't scale with the rectangle
	}
}
