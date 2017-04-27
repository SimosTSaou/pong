#include "Button.h"

Button::Button(sf::Vector2f & size, sf::Vector2f & position, const std::string & button_text, sf::Font & font_used)
{
	button.setSize(size);
	button.setOrigin(button.getLocalBounds().width / 2.0f, button.getLocalBounds().height / 2.0f);
	button.setPosition(position);
	button.setOutlineThickness(1.5f);

	text.setFont(font_used);
	text.setString(button_text);
	text.setOrigin(text.getLocalBounds().width / 2.0f + text.getLocalBounds().left, text.getLocalBounds().height / 2.0f + text.getLocalBounds().top);
	text.setPosition(button.getPosition());
	text.setFillColor(sf::Color::Black);
}

void Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(button);
	target.draw(text);
}

const sf::RectangleShape & Button::getOuterBox() const
{
	return button;
}

void Button::setOutlineColor(const sf::Color passed_color)
{
	button.setOutlineColor(passed_color);
}

void Button::setText(const std::string & passed_text)
{
	text.setString(passed_text);
	text.setOrigin(text.getLocalBounds().width / 2.0f + text.getLocalBounds().left, text.getLocalBounds().height / 2.0f + text.getLocalBounds().top);
	text.setPosition(button.getPosition());
}

const sf::String & Button::getText()
{
	return text.getString();
}