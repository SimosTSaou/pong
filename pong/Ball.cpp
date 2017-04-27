#include "Ball.h"

Ball::Ball(sf::Vector2f & size, sf::Vector2f & position)
{
	ball.setSize(size);
	ball.setOrigin(ball.getLocalBounds().width / 2.0f, ball.getLocalBounds().height / 2.0f);
	ball.setPosition(position);
	ball.setOutlineThickness(1.0f);
	ball.setOutlineColor(sf::Color::Black);
	ball.setFillColor(sf::Color::Black);

	speed = 0.2f;
}

void Ball::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(ball);
}

sf::RectangleShape & Ball::getOuterBox()
{
	return ball;
}