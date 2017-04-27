#include "Cursor.h"
#include <iostream>

Cursor::Cursor(sf::Color & cl)
{
	horizontal.setSize(sf::Vector2f(10.0f, 2.0f));
	horizontal.setFillColor(cl);
	vertical.setSize(sf::Vector2f(2.0f, 10.0f));
	vertical.setFillColor(cl);
	horizontal.setOrigin(horizontal.getSize().x / 2.0f, horizontal.getSize().y / 2.0f);
	vertical.setOrigin(vertical.getSize().x / 2.0f, vertical.getSize().y / 2.0f);

	visible = true;
}

void Cursor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (visible)
	{
		target.draw(horizontal);
		target.draw(vertical);
	}
}

void Cursor::setPosition(sf::Vector2i & coords)
{
	horizontal.setPosition(coords.x, coords.y);
	vertical.setPosition(coords.x, coords.y);
}

void Cursor::setCursorVisible(bool value)
{
	visible = value;
}

bool Cursor::isVisible() const
{
	return visible;
}