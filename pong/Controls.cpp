#include "Controls.h"

extern int g_width;
extern int g_height;
extern int music_volume;
extern int sfx_volume;
extern int muted_sfx_volume;
extern bool muted_sfx;

Controls::Controls()
{
	Button button1(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 3.0f * g_height / 24.0f), "Escape - Open menu", MainMenu::getFont());
	button1.setOutlineColor(sf::Color::White);
	controls.push_back(button1);
	Button button2(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 5.0f * g_height / 24.0f), "W - Left racket up", MainMenu::getFont());
	button2.setOutlineColor(sf::Color::White);
	controls.push_back(button2);
	Button button3(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 7.0f * g_height / 24.0f), "S - Left racket down", MainMenu::getFont());
	button3.setOutlineColor(sf::Color::White);
	controls.push_back(button3);
	Button button4(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 9.0f * g_height / 24.0f), "Up Arrow - Right racket up/ Menu navigation", MainMenu::getFont());
	button4.setOutlineColor(sf::Color::White);
	controls.push_back(button4);
	Button button5(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 11.0f * g_height / 24.0f), "Down Arrow - Right racket down/ Menu navigation", MainMenu::getFont());
	button5.setOutlineColor(sf::Color::White);
	controls.push_back(button5);
	Button button6(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 13.0f * g_height / 24.0f), "P - Pause game", MainMenu::getFont());
	button6.setOutlineColor(sf::Color::White);
	controls.push_back(button6);
	Button button7(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 15.0f * g_height / 24.0f), "R - Restart game", MainMenu::getFont());
	button7.setOutlineColor(sf::Color::White);
	controls.push_back(button7);
	Button button8(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 17.0f * g_height / 24.0f), "M - Mute sounds/music", MainMenu::getFont());
	button8.setOutlineColor(sf::Color::White);
	controls.push_back(button8);
	Button button9(sf::Vector2f(g_width / 2.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 19.0f * g_height / 24.0f), "Enter - Select menu option", MainMenu::getFont());
	button9.setOutlineColor(sf::Color::White);
	controls.push_back(button9);
	Button button10(sf::Vector2f(g_width / 8.0f, g_height / 12.0f), sf::Vector2f(g_width / 2.0f, 22.0f * g_height / 24.0f), "Back", MainMenu::getFont());
	button10.setOutlineColor(sf::Color::Black);
	controls.push_back(button10);
}

int Controls::Run(sf::RenderWindow & render_window, std::vector<sf::Sound> & sounds)
{
	sf::Event event;

	while (render_window.isOpen())
	{
		while (render_window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				render_window.close();

			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					sounds[2].play();
					return 1;
				}
				if (event.key.code == sf::Keyboard::Return)
				{
					sounds[2].play();
					return 1;
				}

				if (event.key.code == sf::Keyboard::M)
					Mute(sounds);
			}

			if (event.type == sf::Event::EventType::MouseButtonPressed)
					return 1;
		}

		cursor.setPosition(sf::Mouse::getPosition(render_window));

		render_window.clear(sf::Color::White);
		for (int i = 0; i < 10; ++i)
			render_window.draw(controls[i]);
		render_window.draw(cursor);
		render_window.display();
	}

	return -1;
}