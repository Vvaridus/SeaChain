#include "scene_options.h"
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

using namespace std;
using namespace sf;

std::shared_ptr<ButtonComponent> btnMusicIncrease;
std::shared_ptr<ButtonComponent> btnMusicDecrease;
std::shared_ptr<ButtonComponent> btnSoundIncrease;
std::shared_ptr<ButtonComponent> btnSoundDecrease;
std::shared_ptr<CheckboxComponent> chkFullscreen;
std::shared_ptr<CheckboxComponent> chkVsync;
std::vector<std::shared_ptr<Entity>> soundIndicator;

void OptionScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

	sf::Vector2f btnDimentions = Vector2f(32, 32);

	auto ins = Data::getInstance();
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
		spritesheet.loadFromFile("resources/textures/SeaChainOptions.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		auto windowSize = Engine::getWindowSize();

		auto background = makeEntity();
		background->addTag("optionMenuImage");
		background->setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
		auto spriteComp = background->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setOrigin(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}
	// create music indicator bar
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/indicatorBar.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);
		auto windowSize = Engine::getWindowSize();
		Vector2f pos = Vector2f(803, 304);

		for (int i = 0; i < 10; i++) {
			auto background = makeEntity();
			background->addTag("musicIndicator");
			background->setPosition(pos);
			auto spriteComp = background->addComponent<SpriteComponent>();
			spriteComp->setTexure(sprite);
			//spriteComp->setOrigin(Vector2f(sprite->getSize().x / 2, sprite->getSize().y / 2));
			pos.x += 31;
		}
	}
	// create volume indicator bar
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/indicatorBar.png", IntRect(0, 0, 1920, 1080));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);
		auto windowSize = Engine::getWindowSize();
		Vector2f pos = Vector2f(803, 396);

		for (int i = 0; i < 10; i++) {
			auto background = makeEntity();
			background->addTag("soundIndicator");
			background->setPosition(pos);
			auto spriteComp = background->addComponent<SpriteComponent>();
			spriteComp->setTexure(sprite);
			//spriteComp->setOrigin(Vector2f(sprite->getSize().x / 2, sprite->getSize().y / 2));
			pos.x += 31;
		}
	}
	//Draw first button (DECREASE MUSIC BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnMusicDecrease");
		button->setPosition(Vector2f(1148, 302));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnMusicDecrease = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnMusicDecrease->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw Second button (INCREASE MUSIC BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnMusicIncrease");
		button->setPosition(Vector2f(1211, 302));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnMusicIncrease = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnMusicIncrease->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw third button (DECREASE VOLUME BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnSoundDecrease");
		button->setPosition(Vector2f(1148, 394));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnSoundDecrease = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnSoundDecrease->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw third button (INCREASE VOLUME BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnSoundIncrease");
		button->setPosition(Vector2f(1211, 394));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnSoundIncrease = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnSoundIncrease->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}

	updateMusicIndicator();
	updateSoundIndicator();

	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");
	setLoaded(true);
}

void OptionScene::Update(const double& dt) {
	if (btnMusicIncrease->isPressed()) {      
		auto ins = Data::getInstance();
		float currentVol = ins->getMusicVolume();
		ins->setMusicVolume(currentVol += 1);
		updateMusicIndicator();
	}
	else if (btnMusicDecrease->isPressed()) {
		auto ins = Data::getInstance();
		float currentVol = ins->getMusicVolume();
		if (currentVol < 1)
			ins->setMusicVolume(0);
		else
			ins->setMusicVolume(--currentVol);
			
		updateMusicIndicator();
	}
	if (btnSoundIncrease->isPressed()) {      
		auto ins = Data::getInstance();
		float currentVol = ins->getSoundVolume();
		ins->setSoundVolume(currentVol += 1);
		updateSoundIndicator();
	}
	else if (btnSoundDecrease->isPressed()) {
		auto ins = Data::getInstance();
		float currentVol = ins->getSoundVolume();
		if (currentVol < 1)
			ins->setSoundVolume(0);
		else
			ins->setSoundVolume(--currentVol);
			
		updateSoundIndicator();
	}

	Scene::Update(dt);
}

void OptionScene::updateMusicIndicator() {
	auto ins = Data::getInstance();
	float currentVol = ins->getMusicVolume();
	auto musicIndicators = this->ents.find("musicIndicator");
	cout << currentVol << endl;

	// if you set music volume to 7 then get the music volume.
	// SFML returns 6.9999964 or something similar which when
	// casting to an int it rounds down, so add 1 to fix this.
	//currentVol++;

	for (auto i : musicIndicators) {
		i->setVisible(true);
	}

	for (int i = musicIndicators.size()-1; i >= currentVol; i--) {
		musicIndicators[i]->setVisible(false);
	}
}

void OptionScene::updateSoundIndicator() {
	auto ins = Data::getInstance();
	float currentVol = ins->getSoundVolume();
	auto soundIndicators = this->ents.find("soundIndicator");
	cout << currentVol << endl;

	// if you set music volume to 7 then get the music volume.
	// SFML returns 6.9999964 or something similar which when
	// casting to an int it rounds down, so add 1 to fix this.
	//currentVol++;

	for (auto i : soundIndicators) {
		i->setVisible(true);
	}

	for (int i = soundIndicators.size() - 1; i >= currentVol; i--) {
		soundIndicators[i]->setVisible(false);
	}
}

void OptionScene::UnLoad() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Unloaded, "");
	Nullify();
	Scene::UnLoad();
}

// Nullify all buttons with a shared_ptr
// Otherwise, engine can't delete the components
// and throws exceptions.
void OptionScene::Nullify() {
	btnMusicIncrease = nullptr;
	btnMusicDecrease = nullptr;
	btnSoundIncrease = nullptr;
	btnSoundDecrease = nullptr;
}