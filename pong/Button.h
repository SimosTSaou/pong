#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <string>

class Button : public sf::Drawable
{
private:
	sf::RectangleShape button;
	sf::Text text;
	void draw(sf::RenderTarget & target, sf::RenderStates states) const;
public:
	Button() {};
	Button(sf::Vector2f & size, sf::Vector2f & position, const std::string & button_text, sf::Font & font_used);
	const sf::RectangleShape & getOuterBox() const;
	void setOutlineColor(const sf::Color);
	void setText(const std::string &);
	const sf::String & getText();
};