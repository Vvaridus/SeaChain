#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("COMIC.TTF");
  _text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str) {
  _string = str;
  _text.setString(_string);
}

std::string TextComponent::getText() {
    return _string;
}

void TextComponent::setCharSize(int s)
{
    _text.setCharacterSize(s);
}

void TextComponent::setOrigin(sf::Vector2f o)
{
    _text.setOrigin(o);
}

void TextComponent::setPosition(sf::Vector2f p)
{
    _text.setPosition(p);
}

void TextComponent::setRotation(float r)
{
    _text.setRotation(r);
}

void TextComponent::setFillColor(sf::Color c)
{
    _text.setFillColor(c);
}

void TextComponent::setStyle(sf::Text::Style s)
{
    _text.setStyle(s);
}

sf::FloatRect TextComponent::getBounds()
{
    return _text.getLocalBounds();
}