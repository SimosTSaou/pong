#pragma once
#include <SFML\Graphics.hpp>

class Cursor : public sf::Drawable
{
private:
	sf::RectangleShape horizontal;
	sf::RectangleShape vertical;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	bool visible;
public:
	Cursor(sf::Color & cl = sf::Color(0, 0, 0));
	void setPosition(sf::Vector2i & coords);
	void setCursorVisible(bool);
	bool isVisible() const;
};