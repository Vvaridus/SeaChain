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

using namespace std;
using namespace sf;

std::shared_ptr<ButtonComponent> btnStart;
std::shared_ptr<ButtonComponent> btnEnd;

void MenuScene::Load() {
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loading, "");

    sf::Vector2f btnDimentions = Vector2f(500, 150);

    auto ins = Data::getInstance();
    auto debug = ins->getDebug();

    // Display background
    {
        Texture spritesheet;
        spritesheet.loadFromFile("resources/mainMenu.png", IntRect(0, 0, 1920, 1080));

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
        button->setVisible(debug);
    
        auto bounds = buttonShape->getBounds();
    
        Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
        btnStart->setBounds(xy, Vector2f(bounds->width, bounds->height));
    }
    //Draw second button (end BUTTON)
    {
        auto button = makeEntity();
        button->addTag("btnEnd");
        button->setPosition(Vector2f(709, 800));
        auto buttonShape = button->addComponent<ShapeComponent>();
        buttonShape->setShape<RectangleShape>(btnDimentions);
        buttonShape->getShape().setFillColor(Color::Transparent);
        buttonShape->getShape().setOutlineThickness(2);
        buttonShape->getShape().setOutlineColor(Color::White);
        button->setVisible(debug);

        auto bounds = buttonShape->getBounds();

        Vector2f xy = Vector2f(button->getPosition().x + (bounds->width / 2), (button->getPosition().y + (bounds->height / 2)));
        btnEnd->setBounds(xy, Vector2f(bounds->width, bounds->height));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    Logger::addEvent(Logger::EventType::Scene, Logger::Action::Loaded, "");
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    if (btnStart->isPressed()) {      
        Nullify();
        Engine::ChangeScene(&tutorialMain);
    }
    else if (btnEnd->isPressed()) {
        Nullify();
        Engine::GetWindow().close();
        std::exit(EXIT_SUCCESS);
    }

    Scene::Update(dt);
}

// Nullify all buttons with a shared_ptr
// Otherwise, engine can't delete the components
// and throws exceptions.
void MenuScene::Nullify() {
    btnStart = nullptr;
    btnEnd = nullptr;
}
