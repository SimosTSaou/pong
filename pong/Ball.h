#pragma once
#include <SFML\Graphics.hpp>

class Ball : public sf::Drawable
{
	friend class Game;
private:
	sf::RectangleShape ball;
	float speed;
	double angle;
	sf::Vector2f direction;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
public:
	Ball() {};
	Ball(sf::Vector2f & size, sf::Vector2f & position);
	sf::RectangleShape & getOuterBox();
};
