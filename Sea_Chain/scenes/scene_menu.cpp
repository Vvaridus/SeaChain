#include "scene_menu.h"
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
#include "../helpers/file_handlers.h"

using namespace std;
using namespace sf;

std::shared_ptr<ButtonComponent> btnStart;
std::shared_ptr<ButtonComponent> btnHelp;
std::shared_ptr<ButtonComponent> btnOptions;
std::shared_ptr<ButtonComponent> btnEnd;

void MenuScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "Main Menu");
	fileHandler::loadKeybinds();
	fileHandler::loadSettings();

	sf::Vector2f btnDimentions = Vector2f(500, 150);

	auto ins = Data::getInstance();
	auto debug = ins->getDebug();
	ins->setPlayer(nullptr);
	ins->setMusicFile("resources/sound/SeaChainTheme.wav");
	ins->setMusicFileBattle("resources/sound/SeaChainBattle.wav");
	ins->playMusic(true);
	ins->setMusicLoop(true);	


	// Display background
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
	//Draw first button (START BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnStart");
		button->setPosition(Vector2f(709, 180));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		buttonShape->setVisibility(debug);

		auto bounds = buttonShape->getBounds();

		btnStart = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnStart->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw third button (OPTIONS BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnHelp");
		button->setPosition(Vector2f(709, 400));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		buttonShape->setVisibility(debug);

		auto bounds = buttonShape->getBounds();

		btnHelp = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnHelp->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw third button (OPTIONS BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnOptions");
		button->setPosition(Vector2f(709, 600));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		buttonShape->setVisibility(debug);

		auto bounds = buttonShape->getBounds();

		btnOptions = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnOptions->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw fouth button (end BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnEnd");
		button->setPosition(Vector2f(709, 800));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		buttonShape->setVisibility(debug);

		auto bounds = buttonShape->getBounds();

		btnEnd = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnEnd->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "Main Menu");
	setLoaded(true);
}

void MenuScene::Update(const double& dt) {
	if (btnStart->isPressed()) {
		Nullify();
		Engine::ChangeScene(&tutorialMain);
	}
	else if (btnHelp->isPressed()) {
		Nullify();
		Engine::ChangeScene(&help);
	}
	else if (btnOptions->isPressed()) {
		Nullify();
		Engine::ChangeScene(&options);
	}
	else if (btnEnd->isPressed()) {
		Nullify();
		Engine::GetWindow().close();
		std::exit(EXIT_SUCCESS);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
		static float debugTrigger = 0.f;
		debugTrigger -= dt;
		if (debugTrigger < 0) {
			auto ins = Data::getInstance();
			ins->setDebug(!ins->getDebug());
			cout << ins->getDebug() << endl;
			debugTrigger = .2f;
		}
	}

	Scene::Update(dt);
}

void MenuScene::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "Main Menu");
	Nullify();
	Scene::UnLoad();
}

// Nullify all buttons with a shared_ptr
// Otherwise, engine can't delete the components
// and throws exceptions.
void MenuScene::Nullify() {
	btnStart = nullptr;
	btnHelp = nullptr;
	btnOptions = nullptr;
	btnEnd = nullptr;
}
