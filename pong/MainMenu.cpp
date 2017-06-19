#include "MainMenu.h" 
#include <iostream>

extern int g_width;
extern int g_height;
extern int music_volume;
extern int sfx_volume;
extern int muted_sfx_volume;
extern bool muted_sfx;
extern int difficulty;
extern bool has_focus;
extern bool playing;

sf::Font MainMenu::font;

MainMenu::MainMenu() : Screen()
{
	font.loadFromFile("calibri.ttf");

	selected_option = 0;

	Button button1(sf::Vector2f(g_width / 4.0f, g_height / 8.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 5 * g_height / 16.0f), "1 Player", font);
	buttons.push_back(button1);
	Button button2(sf::Vector2f(g_width / 4.0f, g_height / 8.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 7 * g_height / 16.0f), "2 Players", font);
	buttons.push_back(button2);
	Button button3(sf::Vector2f(g_width / 4.0f, g_height / 8.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 9 * g_height / 16.0f), "Settings", font);
	buttons.push_back(button3);
	Button button4(sf::Vector2f(g_width / 4.0f, g_height / 8.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 11 * g_height / 16.0f), "Exit", font);
	buttons.push_back(button4);
}

int MainMenu::Run(sf::RenderWindow & render_window, std::vector<sf::Sound> & sounds)
{
	sf::Event event;
	sf::Vector2i mouse_position;
	render_window.setMouseCursorVisible(false);

	while (render_window.isOpen())
	{
		while (render_window.pollEvent(event)) 
		{
			if (event.type == sf::Event::EventType::Closed)
				render_window.close();
			
			if (event.type == sf::Event::LostFocus)
				has_focus = false;
			else if (event.type == sf::Event::GainedFocus)
				has_focus = true;

			if (event.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (buttons[1].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					return 2;
				if (buttons[2].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					return 1;
				if (buttons[3].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					render_window.close();
			}

			if (event.type == sf::Event::EventType::KeyPressed)
			{
				// menu navigation
				if (event.key.code == sf::Keyboard::Up)
				{
					selected_option = (selected_option > 0) ? --selected_option : 3;
					sounds[0].play();
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					selected_option = (selected_option < 3) ? ++selected_option : 0;
					sounds[0].play();
				}

				// menu selection
				if (event.key.code == sf::Keyboard::Return)
				{
					switch (selected_option)
					{
					case 1: sounds[1].play();
						return 2;	// go to game
					case 2: sounds[1].play();
						return 1;	// go to settings
					case 3: render_window.close();	// close the game
					}
				}

				// mute sound
				if (event.key.code == sf::Keyboard::M)
					Mute(sounds);
			}
		}

		mouse_position = sf::Mouse::getPosition(render_window);
		cursor.setPosition(mouse_position);
		
		for (int i = 0; i < 4; ++i)
		{
			buttons[i].setOutlineColor(sf::Color::White);
			if (buttons[i].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(mouse_position)) && has_focus)
			{
				if (i != selected_option)
					sounds[0].play();
				selected_option = i;
			}
		}
		buttons[selected_option].setOutlineColor(sf::Color::Black); 

		render_window.clear(sf::Color::White);
		for (unsigned int i = 0; i < buttons.size(); ++i)
			render_window.draw(buttons[i]);
		render_window.draw(cursor);
		render_window.display();
	}
	
	return -1;
}

sf::Font & MainMenu::getFont() 
{
	return font;
}