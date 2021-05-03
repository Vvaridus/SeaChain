#include "cmp_button.h"
#include <system_renderer.h>
#include <system_resources.h>
#include "../components/cmp_sprite.h"
#include <engine.h>
#include "maths.h"
#include <logger.h>

void ButtonComponent::update(double dt) {
	// Set it to not pressed, in most this is to reset the button once clicked.
	_state = ButtonState::BTN_IDLE;

	// Get the mouse position based off of game window
	//sf::Vector2i mousePos = sf::Mouse::getPosition(Engine::GetWindow()); // this does not get position relative to scale
	sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos); // this gets world pos which is mouse position relative to scale
	// get top left of the button (This is presuming origin is always middle. Could do with rework)
	sf::Vector2f topLeft = sf::Vector2f(_pos.x - (_size.x / 2), _pos.y - (_size.y / 2));

	// Cool down on pressing
	static float triggertime = 0.0f;
	triggertime -= dt;

	if (triggertime <= 0) {
		// Check mouse is within the button
		if (_allowInteraction)
		{
			if (worldPos.x > topLeft.x && worldPos.x < (topLeft.x + _size.x) &&
				worldPos.y > topLeft.y && worldPos.y < (topLeft.y + _size.y)) {

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// Set button to active
					_state = ButtonState::BTN_ACTIVE;

					triggertime = 1.2f;
				}
			}
		}
	}
}

void ButtonComponent::render() { }

// Function for setting the position of the button and size of the button (Or just button, doesn't require shape)


// Origin position must be the centre.
void ButtonComponent::setBounds(sf::Vector2f position, sf::Vector2f size)
{
	_pos = position;
	_size = size;
}

// Origin position must be the centre.
void ButtonComponent::setBounds(sf::Vector2f position)
{
	_pos = position;
}

ButtonComponent::ButtonComponent(Entity* const p)
	: Component(p), _allowInteraction(true) {
}

// For getting ID
const std::string& ButtonComponent::getID() const {
	return _id;
}

// For setting ID
void ButtonComponent::setID(std::string id) {
	_id = id;
}

// Set whether or not the button can be used
void ButtonComponent::setInteraction(bool interaction) {
	_allowInteraction = interaction;
}

// Check if button is pressed
const bool ButtonComponent::isPressed() const {
	// if the state is active return true

	// if there is a read access violation
	// The button does not exist or not defined properly.
	if (_state == ButtonState::BTN_ACTIVE)
		return true;
	else
		return false;
}

void CheckboxComponent::update(double dt) {

	// Get the mouse position based off of game window
	//sf::Vector2i mousePos = sf::Mouse::getPosition(Engine::GetWindow()); // this does not get position relative to scale
	sf::Vector2i pixelPos = sf::Mouse::getPosition(Engine::GetWindow());
	sf::Vector2f worldPos = Engine::GetWindow().mapPixelToCoords(pixelPos); // this gets world pos which is mouse position relative to scale
	// get top left of the checkbox (This is presuming origin is always middle. Could do with rework)
	sf::Vector2f topLeft = sf::Vector2f(_pos.x - (_size.x / 2), _pos.y - (_size.y / 2));

	// Cool down on pressing
	static float triggertime = 0.0f;
	triggertime -= dt;

	if (triggertime <= 0) {
		// Check mouse is within the checkbox
		if (_allowInteraction)
		{
			if (worldPos.x > topLeft.x && worldPos.x < (topLeft.x + _size.x) &&
				worldPos.y > topLeft.y && worldPos.y < (topLeft.y + _size.y)) {

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// Set button to active
					_state = !_state;

					triggertime = 1.2f;
				}
			}
		}
	}
}

void CheckboxComponent::render() { }

// Function for setting the position of the checkbox and size of the checkbox (Or just Checkbox, doesn't require shape)
// Origin position must be the centre.
void CheckboxComponent::setBounds(sf::Vector2f position, sf::Vector2f size)
{
	_pos = position;
	_size = size;
}

// Origin position must be the centre.
void CheckboxComponent::setBounds(sf::Vector2f position)
{
	_pos = position;
}

CheckboxComponent::CheckboxComponent(Entity* const p)
	: Component(p), _allowInteraction(true), _state(false) {
}

// For getting ID
const std::string& CheckboxComponent::getID() const {
	return _id;
}

// For setting ID
void CheckboxComponent::setID(std::string id) {
	_id = id;
}

void CheckboxComponent::setInteraction(bool interaction) {
	_allowInteraction = interaction;
}

// Check if checkbox is checked
const bool CheckboxComponent::isChecked() const {
	// if the state is active return true

	// if there is a read access violation
	// The checkbox does not exist or not defined properly.
	return _state;
}