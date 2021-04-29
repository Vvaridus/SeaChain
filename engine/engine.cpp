#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>
#include <logger.h>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;
bool Engine::_fullscreen = false;
bool Engine::_vsync = false;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

static Vector2f pos = Vector2f(-500, 540); // position of loading sprites
// loading sprite
static Texture _shipSprite; 
static Texture _textSprite;
// whether or not the loading textures have been loaded.
static bool _loadTextures;

void loadTextures() {
	// load ship texture
	Texture ship;
	ship.loadFromFile("resources/textures/SeaChainLoadingShip.png", IntRect(0, 0, 1920, 1080));
	_shipSprite = ship;

	// load text texture
	Texture text;
	text.loadFromFile("resources/textures/SeaChainLoadingText.png", IntRect(0, 0, 1920, 1080));
	_textSprite = text;

	// set the textures to loaded
	_loadTextures = true;
}

void Loading_update(float dt, const Scene* const scn) {
	if (scn->isLoaded()) {
		Logger::addEvent(Logger::EventType::Engine, Logger::Action::Leaving_Loading_Screen, "");
		loading = false;
	}
	else {
		pos += Vector2f(500.0f * dt, 0);
		loadingTime += dt;
	}
}
void Loading_render() {
	auto windowSize = Engine::getWindowSize();

	// Create ship sprite, set the position, set the origin to centre point
	static Sprite ship;
	ship.setTexture(_shipSprite);
	ship.setPosition(pos);
	ship.setOrigin(Vector2f(_shipSprite.getSize().x / 2, _shipSprite.getSize().y / 2));

	// create text sprite, if position is within 200+/- of the centre lock it's position to exactly the centre
	// set origin to centre point. 
	static Sprite text;
	text.setTexture(_textSprite);

	if (text.getPosition().x > ((windowSize.x / 2) - 10) && text.getPosition().x < ((windowSize.x / 2) + 10)) {
		text.setPosition(Vector2f(windowSize.x / 2, windowSize.y / 2));
	}
	else {
		text.setPosition(Vector2f(pos.x, windowSize.y / 2));
	}

	text.setOrigin(Vector2f(_textSprite.getSize().x / 2, _textSprite.getSize().y / 2));

	Renderer::queue(&ship);
	Renderer::queue(&text);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		if (ftc % 60 == 0) {
			double davg = 0;
			for (const auto t : frametimes) {
				davg += t;
			}
			davg = 1.0 / (davg / 255.0);
			_window->setTitle(avg + toStrDecPt(2, davg));
		}
	}

	if (loading) {
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		_loadTextures = false;
		pos = Vector2f(-500, 540);
		Physics::update(dt);
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {

	if (loading) {
		if (!_loadTextures)
			loadTextures();

		Loading_render();
	}
	else if (_activeScene != nullptr) {
		_activeScene->Render();
	}

	Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn) {
	RenderWindow window(VideoMode(width, height), gameName, sf::Style::Titlebar | sf::Style::Close);
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);
	Physics::initialise();
	ChangeScene(scn);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		//if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		//	window.close();
		//}

		window.clear();
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	window.close();
	Physics::shutdown();
	// Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

void Engine::setVsync(bool b) { 
	_window->setVerticalSyncEnabled(b); 
	_vsync = b;
}

bool Engine::getVsync() { return _vsync; }

void Engine::setFramerate(int fps) { _window->setFramerateLimit(fps); }

bool Engine::getFullscreen() { return _fullscreen; }

void Engine::setFullscreen(bool fullscreen) {
	if (fullscreen)
	{
		_fullscreen = true;
	}
	else
	{
		_fullscreen = false;
	}
}

void Engine::ChangeScene(Scene* s) {
	Logger::addEvent(Logger::EventType::Engine, Logger::Action::Scene_Change, "");
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr) {
		old->UnLoad(); // todo: Unload Async
	}

	if (!s->isLoaded()) {
		Logger::addEvent(Logger::EventType::Engine, Logger::Action::Entering_Loading_Screen, "");
		loadingTime = 0;
		_activeScene->LoadAsync();
		loading = true;
	}
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		// Are we already loading asynchronously?
		if (_loaded_future.valid() // yes
			&&                     // Has it finished?
			_loaded_future.wait_for(chrono::seconds(0)) ==
			future_status::ready) {
			// Yes
			_loaded_future.get();
			_loaded = true;
		}
		return _loaded;
	}
}
void Scene::setLoaded(bool b) {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		_loaded = b;
	}
}

void Scene::UnLoad() {
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
	// Return time since Epoc
	long long now() {
		return std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count();
	}
	// Return time since last() was last called.
	long long last() {
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
} // namespace timing

Scene::~Scene() { UnLoad(); }