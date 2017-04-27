#include "Bracket.h"

Bracket::Bracket(sf::Vector2f & size, sf::Vector2f & starting_position)
{
	bracket.setSize(size);
	bracket.setOrigin(bracket.getLocalBounds().width / 2.0f, bracket.getLocalBounds().height / 2.0f);
	bracket.setPosition(starting_position);
	bracket.setOutlineThickness(1.5f);
	bracket.setOutlineColor(sf::Color::Black);

	speed = 0.3;
}

void Bracket::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(bracket);
}

sf::RectangleShape & Bracket::getOuterBox()
{
	return bracket;
}
