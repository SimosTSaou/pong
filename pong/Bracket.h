#pragma once
#include <SFML\Graphics.hpp>

class Bracket : public sf::Drawable
{
	friend class Game;
private:
	sf::RectangleShape bracket;
	float speed;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
public:
	Bracket() {};
	Bracket(sf::Vector2f & size, sf::Vector2f & starting_position);
	sf::RectangleShape & getOuterBox();
};
