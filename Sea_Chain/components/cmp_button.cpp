#include "cmp_button.h"
#include <system_renderer.h>
#include <system_resources.h>
#include "../components/cmp_sprite.h"
#include <engine.h>
#include "maths.h"

void ButtonComponent::update(double dt) {
	// Set it to not pressed, in most this is to reset the button once clicked.
	_state = ButtonState::BTN_IDLE;

	// Get the mouse position based off of game window
	sf::Vector2i mousePos = sf::Mouse::getPosition(Engine::GetWindow());
	// get top left of the button (This is presuming origin is always middle. Could do with rework)
	sf::Vector2f topLeft = sf::Vector2f(_pos.x - (_size.x / 2), _pos.y - (_size.y / 2));
	
	// Cool down on pressing
	static float triggertime = 0.0f;
	triggertime -= dt;

	if(triggertime <= 0){
		// Check mouse is within the button
		if (mousePos.x > topLeft.x && mousePos.x < (topLeft.x + _size.x) &&
			mousePos.y > topLeft.y && mousePos.y < (topLeft.y + _size.y)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				// Set button to active
				_state = ButtonState::BTN_ACTIVE;

				triggertime = .5f;
			}
		}		
	}
}

void ButtonComponent::render() { }

// Function for setting the position of the button and size of the button (Or just button, doesn't require shape)
void ButtonComponent::setBounds(sf::Vector2f position, sf::Vector2f size)
{
	_pos = position;
	_size = size;
}

ButtonComponent::ButtonComponent(Entity* const p)
	: Component(p) {
}

// For getting ID
const std::string& ButtonComponent::getID() const {
	return _id;
}

// For setting ID
void ButtonComponent::setID(std::string id){
	_id = id;
}

// Check if button is pressed
const bool ButtonComponent::isPressed() const {
	// if the state is active return true
	if (_state == ButtonState::BTN_ACTIVE)
		return true;
	else
		return false;
}

