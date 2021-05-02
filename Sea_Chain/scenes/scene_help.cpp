#include "scene_help.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <logger.h>
#include "../components/cmp_sprite.h"
#include <SFML/Graphics.hpp>
#include "../components/cmp_button.h"
#include "../gameData.h"
#include <SFML/Audio/Music.hpp>
#include "../helpers/convert.h"

using namespace std;
using namespace sf;

std::shared_ptr<ButtonComponent> btnBack;

void HelpScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

	auto windowSize = Engine::getWindowSize();

	auto ins = Data::getInstance();
	auto keybinds = ins->getKeybinds();
	auto debug = ins->getDebug();

	// Display background main menu
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/mainMenu.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		auto windowSize = Engine::getWindowSize();

		auto background = makeEntity();
		background->addTag("backgroundImage");
		background->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = background->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}
	// overlay the options menu ontop of the main menu one
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/HelpMenu.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		auto windowSize = Engine::getWindowSize();

		auto background = makeEntity();
		background->addTag("HelpMenu");
		background->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = background->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}
	//Draw first button (BACK BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnBack");
		button->setPosition(Vector2f(120, 950));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(Vector2f(250, 72));
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		buttonShape->setVisibility(debug);

		auto bounds = buttonShape->getBounds();

		btnBack = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnBack->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");
	setLoaded(true);
}

void HelpScene::Update(const double& dt) {
	bool changingScenes = false;
	auto ins = Data::getInstance();
	auto keybinds = ins->getKeybinds();

	if (sf::Keyboard::isKeyPressed(keybinds->find("GO_BACK")->second)) {
		Engine::ChangeScene(&menu);
	}
	if (btnBack->isPressed())
	{
		changingScenes = true;
		Engine::ChangeScene(&menu);
	}

	if(changingScenes == false)
		Scene::Update(dt);
}

void HelpScene::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
	Nullify();
	Scene::UnLoad();
}

// Nullify all buttons with a shared_ptr
// Otherwise, engine can't delete the components
// and throws exceptions.
void HelpScene::Nullify() {
	btnBack = nullptr;
}