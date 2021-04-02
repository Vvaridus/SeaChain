#include "tutorial_main.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_basic_movement.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void TutorialMain::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("resources/map.txt", 54);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));

  // Create player
  {
    player = makeEntity();
    player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    s->getShape().setFillColor(Color::Magenta);
    s->getShape().setOrigin(10.f, 15.f);
    auto b = player->addComponent<BasicMovementComponent>();
    b->setSpeed(5);
    //player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
  }

  //Simulate long loading times
   std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void TutorialMain::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void TutorialMain::Update(const double& dt) {

  if (ls::getTileAt(player->getPosition()) == ls::END) {

    //Engine::ChangeScene((Scene*)&level2);
  }

  Scene::Update(dt);
}

void TutorialMain::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
