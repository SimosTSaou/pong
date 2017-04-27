// MainMenu class, derives from Screen
// it's the screen that appears when the application starts, you select the number of players (1 or 2), and enter the name(s)
// you can return to the main menu through the menu you can open while playing the game to start again from the beginning
#pragma once
#include "Screen.h"
#include "Cursor.h"
#include "Button.h"
#include "Settings.h"
#include <vector>

class MainMenu : public Screen
{
private:
	int selected_option;
	Cursor cursor;
	std::vector<Button> buttons;
	static sf::Font font;
public:
	MainMenu();
	virtual int Run(sf::RenderWindow &, std::vector<sf::Sound> &);
	static sf::Font & getFont();
};

