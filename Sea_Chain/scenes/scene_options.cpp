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
#include "../helpers/convert.h"

using namespace std;
using namespace sf;

std::shared_ptr<ButtonComponent> btnMusicIncrease;
std::shared_ptr<ButtonComponent> btnMusicDecrease;
std::shared_ptr<ButtonComponent> btnSoundIncrease;
std::shared_ptr<ButtonComponent> btnSoundDecrease;
std::shared_ptr<ButtonComponent> btnFpsIncrease;
std::shared_ptr<ButtonComponent> btnFpsDecrease;
std::shared_ptr<ButtonComponent> btnUpKeybind;
std::shared_ptr<ButtonComponent> btnDownKeybind;
std::shared_ptr<ButtonComponent> btnLeftKeybind;
std::shared_ptr<ButtonComponent> btnRightKeybind;
std::shared_ptr<ButtonComponent> btnUseKeybind;
std::shared_ptr<ButtonComponent> btnBackKeybind;
std::shared_ptr<ButtonComponent> btnGoBack;
std::shared_ptr<CheckboxComponent> chkFullscreen;
std::shared_ptr<CheckboxComponent> chkVsync;

static bool recordKey = false;
static bool lockFpsSetting = false;

void OptionScene::Load() {
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

	sf::Vector2f btnDimentions = Vector2f(32, 32);
	sf::Vector2f chkDimentions = Vector2f(64, 64);
	sf::Vector2f btnKeybindDimentions = Vector2f(64, 64);

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
	//Draw fourth button (INCREASE VOLUME BUTTON)
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
	// Draw first checkbox (FULL SCREEN CHECKBOX)
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/TickMarker.png", IntRect(0, 0, 61, 47));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		auto checkbox = makeEntity();
		checkbox->addTag("chkFullscreen");
		checkbox->setPosition(Vector2f(696, 555));
		auto chkShape = checkbox->addComponent<ShapeComponent>();
		chkShape->setShape<RectangleShape>(chkDimentions);
		chkShape->getShape().setFillColor(Color::Transparent);
		chkShape->getShape().setOutlineThickness(2);
		chkShape->getShape().setOutlineColor(Color::White);
		auto spriteComp = checkbox->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setVisibility(false);
		checkbox->setVisible(debug);

		auto bounds = chkShape->getBounds();

		chkFullscreen = checkbox->addComponent<CheckboxComponent>();
		Vector2f xy = Vector2f(checkbox->getPosition().x + (bounds->width / 2), (checkbox->getPosition().y + (bounds->height / 2)));
		chkFullscreen->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	// Draw first checkbox (VSYNC CHECKBOX)
	{
		Texture spritesheet;
		spritesheet.loadFromFile("resources/textures/TickMarker.png", IntRect(0, 0, 61, 47));

		shared_ptr<Texture> sprite = make_shared<Texture>(spritesheet);

		auto checkbox = makeEntity();
		checkbox->addTag("chkVsync");
		checkbox->setPosition(Vector2f(831, 555));
		auto chkShape = checkbox->addComponent<ShapeComponent>();
		chkShape->setShape<RectangleShape>(chkDimentions);
		chkShape->getShape().setFillColor(Color::Transparent);
		chkShape->getShape().setOutlineThickness(2);
		chkShape->getShape().setOutlineColor(Color::White);
		auto spriteComp = checkbox->addComponent<SpriteComponent>();
		spriteComp->setTexure(sprite);
		spriteComp->setVisibility(false);
		checkbox->setVisible(debug);

		auto bounds = chkShape->getBounds();

		chkVsync = checkbox->addComponent<CheckboxComponent>();
		Vector2f xy = Vector2f(checkbox->getPosition().x + (bounds->width / 2), (checkbox->getPosition().y + (bounds->height / 2)));
		chkVsync->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw fifth button (DECREASE FPS BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnFpsDecrease");
		button->setPosition(Vector2f(927, 570));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnFpsDecrease = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnFpsDecrease->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw sixth button (INCREASE FPS BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnFpsIncrease");
		button->setPosition(Vector2f(1211, 570));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnFpsIncrease = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnFpsIncrease->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	// Draw textbox for the FPS
	{
		auto text = makeEntity();
		text->addTag("fpsText");
		text->setPosition(Vector2f(1065, 555));
		auto textBox = text->addComponent<TextComponent>("0");
		textBox->setFillColor(Color::White);
		textBox->setCharSize(32);
		textBox->setPosition(text->getPosition());
		//textBox->setOrigin(Vector2f(textBox->getBounds().width / 2, textBox->getBounds().height / 2));
	}
	//Draw seventh button (CONTROL UP KEYBIND BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnUpKeybind");
		button->setPosition(Vector2f(985, 792));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnKeybindDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);
		auto txt = button->addComponent<TextComponent>(Converter::SFKeyToString(keybinds->find("MOVE_UP")->second));
		txt->setFillColor(Color::White);
		txt->setCharSize(32);
		txt->setPosition(Vector2f(button->getPosition().x + 20, button->getPosition().y + 20));
		auto bounds = buttonShape->getBounds();

		btnUpKeybind = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnUpKeybind->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw eighth button (CONTROL DOWN KEYBIND BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnDownKeybind");
		button->setPosition(Vector2f(985, 875));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnKeybindDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		auto txt = button->addComponent<TextComponent>(Converter::SFKeyToString(keybinds->find("MOVE_DOWN")->second));
		txt->setFillColor(Color::White);
		txt->setCharSize(32);
		txt->setPosition(Vector2f(button->getPosition().x + 20, button->getPosition().y + 20));
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnDownKeybind = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnDownKeybind->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw ninth button (CONTROL LEFT KEYBIND BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnLeftKeybind");
		button->setPosition(Vector2f(901, 875));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnKeybindDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		auto txt = button->addComponent<TextComponent>(Converter::SFKeyToString(keybinds->find("MOVE_LEFT")->second));
		txt->setFillColor(Color::White);
		txt->setCharSize(32);
		txt->setPosition(Vector2f(button->getPosition().x + 20, button->getPosition().y + 20));
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnLeftKeybind = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnLeftKeybind->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw tenth button (CONTROL RIGHT KEYBIND BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnRightKeybind");
		button->setPosition(Vector2f(1069, 875));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnKeybindDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		auto txt = button->addComponent<TextComponent>(Converter::SFKeyToString(keybinds->find("MOVE_RIGHT")->second));
		txt->setFillColor(Color::White);
		txt->setCharSize(32);
		txt->setPosition(Vector2f(button->getPosition().x + 20, button->getPosition().y + 20));
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnRightKeybind = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnRightKeybind->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw eleventh button (CONTROL USE KEYBIND BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnUseKeybind");
		button->setPosition(Vector2f(1106, 744));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnKeybindDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		auto txt = button->addComponent<TextComponent>(Converter::SFKeyToString(keybinds->find("INTERACT")->second));
		txt->setFillColor(Color::White);
		txt->setCharSize(32);
		txt->setPosition(Vector2f(button->getPosition().x + 20, button->getPosition().y + 20));
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnUseKeybind = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnUseKeybind->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw Twelfth button (CONTROL BACK KEYBIND BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnBackKeybind");
		button->setPosition(Vector2f(696, 858));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(btnKeybindDimentions);
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		auto txt = button->addComponent<TextComponent>(Converter::SFKeyToString(keybinds->find("GO_BACK")->second));
		txt->setFillColor(Color::White);
		txt->setCharSize(32);
		txt->setPosition(Vector2f(button->getPosition().x + 20, button->getPosition().y + 20));
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnBackKeybind = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnBackKeybind->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}
	//Draw Thirteenth button (BACK BUTTON)
	{
		auto button = makeEntity();
		button->addTag("btnBack");
		button->setPosition(Vector2f(630, 934));
		auto buttonShape = button->addComponent<ShapeComponent>();
		buttonShape->setShape<RectangleShape>(Vector2f(200, 72));
		buttonShape->getShape().setFillColor(Color::Transparent);
		buttonShape->getShape().setOutlineThickness(2);
		buttonShape->getShape().setOutlineColor(Color::White);
		button->setVisible(debug);

		auto bounds = buttonShape->getBounds();

		btnGoBack = button->addComponent<ButtonComponent>();
		Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
		btnGoBack->setBounds(xy, Vector2f(bounds->width, bounds->height));
	}


	updateMusicIndicator();
	updateSoundIndicator();

	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");
	setLoaded(true);
}

void OptionScene::Update(const double& dt) {
	bool changingScenes = false;
	auto ins = Data::getInstance();
	auto keybinds = ins->getKeybinds();

	if (btnMusicIncrease->isPressed()) {
		auto ins = Data::getInstance();
		// if current volume is greater than 9 set it to 10
		// sfml returns weird floats at times
		// Set the volume to the current volume take away 1.
		// Then update the volume bar
		float currentVol = ins->getMusicVolume();
		if (currentVol > 9)
			ins->setMusicVolume(10);
		else
			ins->setMusicVolume(++currentVol);
		updateMusicIndicator();
	}
	else if (btnMusicDecrease->isPressed()) {
		auto ins = Data::getInstance();
		// if current volume is less than 1 set it to 0
		// sfml returns weird floats at times
		// Set the volume to the current volume plus 1.
		// Then update the volume bar
		float currentVol = ins->getMusicVolume();
		if (currentVol < 1)
			ins->setMusicVolume(0);
		else
			ins->setMusicVolume(--currentVol);

		updateMusicIndicator();
	}
	else if (btnSoundIncrease->isPressed()) {
		auto ins = Data::getInstance();
		// if current volume is greater than 9 set it to 10
		// sfml returns weird floats at times
		// Set the volume to the current volume take away 1.
		// Then update the volume bar
		float currentVol = ins->getSoundVolume();
		if (currentVol > 9)
			ins->setSoundVolume(10);
		else
			ins->setSoundVolume(++currentVol);
		updateSoundIndicator();
	}
	else if (btnSoundDecrease->isPressed()) {
		auto ins = Data::getInstance();
		// if current volume is less than 1 set it to 0
		// sfml returns weird floats at times
		// Set the volume to the current volume plus 1.
		// Then update the volume bar
		float currentVol = ins->getSoundVolume();
		if (currentVol < 1)
			ins->setSoundVolume(0);
		else
			ins->setSoundVolume(--currentVol);

		updateSoundIndicator();
	}
	else if (chkFullscreen->isChecked()) {
		auto chk = this->ents.find("chkFullscreen")[0];
		auto chkSprite = chk->GetCompatibleComponent<SpriteComponent>()[0];
		chkSprite->setVisibility(true);

	}
	else if (btnFpsIncrease->isPressed() && lockFpsSetting == false) {
		// get the text entity and text component
		auto txt = this->ents.find("fpsText")[0];
		auto txtbox = txt->GetCompatibleComponent<TextComponent>()[0];
		// get the current fps in the textbox
		// add 5 to the current fps then set the text and framerate
		// to the new fps.
		int fps = std::stoi(txtbox->getText());
		fps += 15;
		txtbox->SetText(std::to_string(fps));
		Engine::setFramerate(fps);
	}
	else if (btnFpsDecrease->isPressed() && lockFpsSetting == false) {
		// get the text entity and text component
		auto txt = this->ents.find("fpsText")[0];
		auto txtbox = txt->GetCompatibleComponent<TextComponent>()[0];
		// get the current fps in the textbox
		// add 5 to the current fps then set the text and framerate
		// to the new fps.
		int fps = std::stoi(txtbox->getText());
		fps -= 15;
		txtbox->SetText(std::to_string(fps));
		Engine::setFramerate(fps);
	}
	else if (btnUpKeybind->isPressed()) {
		if (recordKey) {
			auto btn = this->ents.find("btnUpKeybind")[0];
			auto txt = btn->GetCompatibleComponent<TextComponent>()[0];
			sf::Keyboard::Key key = Converter::UInt32ToSFKey(Engine::getPressedKey());
			txt->SetText(Converter::SFKeyToString(key));
			ins->setKeybind("MOVE_UP", key);
			recordKey = false;
		}
		else
			recordKey = true;
	}
	else if (btnDownKeybind->isPressed()) {
		if (recordKey) {
			auto btn = this->ents.find("btnDownKeybind")[0];
			auto txt = btn->GetCompatibleComponent<TextComponent>()[0];
			sf::Keyboard::Key key = Converter::UInt32ToSFKey(Engine::getPressedKey());
			txt->SetText(Converter::SFKeyToString(key));
			ins->setKeybind("MOVE_DOWN", key);
			recordKey = false;
		}
		else
			recordKey = true;
	}
	else if (btnLeftKeybind->isPressed()) {
		if (recordKey) {
			auto btn = this->ents.find("btnLeftKeybind")[0];
			auto txt = btn->GetCompatibleComponent<TextComponent>()[0];
			sf::Keyboard::Key key = Converter::UInt32ToSFKey(Engine::getPressedKey());
			txt->SetText(Converter::SFKeyToString(key));
			ins->setKeybind("MOVE_LEFT", key);
			recordKey = false;
		}
		else
			recordKey = true;
	}
	else if (btnRightKeybind->isPressed()) {
		if (recordKey) {
			auto btn = this->ents.find("btnRightKeybind")[0];
			auto txt = btn->GetCompatibleComponent<TextComponent>()[0];
			sf::Keyboard::Key key = Converter::UInt32ToSFKey(Engine::getPressedKey());
			txt->SetText(Converter::SFKeyToString(key));
			ins->setKeybind("MOVE_RIGHT", key);
			recordKey = false;
		}
		else
			recordKey = true;
	}
	else if (btnUseKeybind->isPressed()) {
		if (recordKey) {
			auto btn = this->ents.find("btnUseKeybind")[0];
			auto txt = btn->GetCompatibleComponent<TextComponent>()[0];
			sf::Keyboard::Key key = Converter::UInt32ToSFKey(Engine::getPressedKey());
			txt->SetText(Converter::SFKeyToString(key));
			ins->setKeybind("INTERACT", key);
			recordKey = false;
		}
		else
			recordKey = true;
	}
	else if (btnBackKeybind->isPressed()) {
		if (recordKey) {
			auto btn = this->ents.find("btnBackKeybind")[0];
			auto txt = btn->GetCompatibleComponent<TextComponent>()[0];
			sf::Keyboard::Key key = Converter::UInt32ToSFKey(Engine::getPressedKey());
			txt->SetText(Converter::SFKeyToString(key));
			ins->setKeybind("GO_BACK", key);
			recordKey = false;
		}
		else
			recordKey = true;
	}
	if (btnGoBack->isPressed()) {
		changingScenes = true;
		Engine::ChangeScene(&menu);
	}
	if (changingScenes == false && sf::Keyboard::isKeyPressed(keybinds->find("GO_BACK")->second)) {
		Engine::ChangeScene(&menu);
	}


	if (changingScenes == false && chkVsync->isChecked()) {
		// if the checkbox IS checked
		// Make the check mark visible
		auto chk = this->ents.find("chkVsync")[0];
		auto chkSprite = chk->GetCompatibleComponent<SpriteComponent>()[0];
		chkSprite->setVisibility(true);

		// if vsync is not currently set, set it.
		if (!Engine::getVsync())
		{
			Engine::setVsync(true);
			// Also disable framerate limit, SFML suggests doing this.
			// set bool to lock it from being changed
			Engine::setFramerate(0);
			lockFpsSetting = true;
		}
	}
	else if (changingScenes == false && !chkVsync->isChecked()) {
		// if the checkbox IS NOT checked
		// hide the check mark
		auto chk = this->ents.find("chkVsync")[0];
		auto chkSprite = chk->GetCompatibleComponent<SpriteComponent>()[0];
		chkSprite->setVisibility(false);
		// if vsync is enabled set it to not enabled.
		if (Engine::getVsync())
		{
			Engine::setVsync(false);
			// allow fps to be changed.
			lockFpsSetting = false;
		}
	}


	
	if(changingScenes == false)
		Scene::Update(dt);
}

void OptionScene::updateMusicIndicator() {
	auto ins = Data::getInstance();
	float currentVol = ins->getMusicVolume(); // Get the current volume
	auto musicIndicators = this->ents.find("musicIndicator"); // get all music indicator entities

	for (auto i : musicIndicators) { // set them all to visible to make next part easier.
		i->setVisible(true);
	}
	// loop through the list backwards setting visibility to false till you reach the current volume
	// then stop setting them to 
	for (int i = musicIndicators.size() - 1; i >= currentVol; i--) {
		musicIndicators[i]->setVisible(false);
	}
}

void OptionScene::updateSoundIndicator() {
	auto ins = Data::getInstance();
	// Get the current volume
	float currentVol = ins->getSoundVolume();
	// get all music indicator entities
	auto soundIndicators = this->ents.find("soundIndicator");

	// set them all to visible to make next part easier.
	for (auto i : soundIndicators) {
		i->setVisible(true);
	}
	// loop through the list backwards setting visibility to false till you reach the current volume
	// then stop setting them to 
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
	btnFpsIncrease = nullptr;
	btnFpsDecrease = nullptr;
	chkFullscreen = nullptr;
	chkVsync = nullptr;
	btnUpKeybind = nullptr;
	btnDownKeybind = nullptr;
	btnLeftKeybind = nullptr;
	btnRightKeybind = nullptr;
	btnBackKeybind = nullptr;
	btnUseKeybind = nullptr;
	btnGoBack = nullptr;
}