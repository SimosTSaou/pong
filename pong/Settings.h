#pragma once
#include "Screen.h"
#include "Button.h"
#include "Cursor.h"
#include "MainMenu.h"
#include <SFML\Graphics.hpp>
#include <vector>

class Settings : public Screen
{
private:
	Cursor cursor;
	int setting_selected;
	std::vector<Button> setting;
	sf::RectangleShape music_volume_background_helper;
	sf::RectangleShape music_volume_background;
	sf::RectangleShape music_volume_adjuster;
	sf::RectangleShape sfx_volume_background_helper;
	sf::RectangleShape sfx_volume_background;
	sf::RectangleShape sfx_volume_adjuster;
	sf::RectangleShape difficulty_background_helper;
	sf::RectangleShape difficulty_background;
	sf::RectangleShape difficulty_adjuster;
public:
	Settings();
	virtual int Run(sf::RenderWindow &, std::vector<sf::Sound> &);
	void Mute(std::vector<sf::Sound> &);
};
