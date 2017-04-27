#pragma once
#include "MainMenu.h"
#include "Button.h"
#include "Cursor.h"
#include <vector>

class Controls : public Screen
{
private:
	Cursor cursor;
	std::vector<Button> controls;
public:
	Controls();
	virtual int Run(sf::RenderWindow &, std::vector<sf::Sound> &);
};
