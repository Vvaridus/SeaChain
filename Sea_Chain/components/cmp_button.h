#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class ButtonComponent : public Component {
public:
	// Different states of the button
	// Could add hover and change colours etc
	enum class ButtonState {
		BTN_IDLE,
		BTN_ACTIVE
	};

	ButtonComponent() = delete;
	explicit ButtonComponent(Entity* p);
	void update(double dt) override;
	void render() override;
	~ButtonComponent() = default;

	const bool isPressed() const;
	const std::string& getID() const;
	void setInteraction(bool interaction);
	void setID(std::string id);
	// Origin position must be the centre.
	void setBounds(sf::Vector2f position, sf::Vector2f size);
	// Origin position must be the centre.
	void setBounds(sf::Vector2f position);
protected:
	// storing the buttons current state
	ButtonState _state; 
	// Storing ID
	std::string _id;
	// Storing the position of the button (Presuming origin is the centre)
	sf::Vector2f _pos;
	// size of the button
	sf::Vector2f _size;
	bool _allowInteraction;
};

class CheckboxComponent : public Component {
public:

	CheckboxComponent() = delete;
	explicit CheckboxComponent(Entity* p);
	void update(double dt) override;
	void render() override;
	~CheckboxComponent() = default;

	const bool isChecked() const;
	const std::string& getID() const;
	void setInteraction(bool interaction);
	void setID(std::string id);
	// Origin position must be the centre.
	void setBounds(sf::Vector2f position, sf::Vector2f size);
	// Origin position must be the centre.
	void setBounds(sf::Vector2f position);
protected:
	// storing the buttons current state
	bool _state;
	// Storing ID
	std::string _id;
	// Storing the position of the button (Presuming origin is the centre)
	sf::Vector2f _pos;
	// size of the button
	sf::Vector2f _size;
	bool _allowInteraction;
};
