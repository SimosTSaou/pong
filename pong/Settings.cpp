#include "Settings.h"
#include <string>
#include <iostream>

extern int g_width;
extern int g_height;
extern int music_volume;
extern int sfx_volume;
extern int muted_sfx_volume;
extern bool muted_sfx;
extern int difficulty;
extern bool has_focus;


Settings::Settings()
{
	setting_selected = 0;

	Button button1(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 2.0f * g_height / 10.0f), "Controls", MainMenu::getFont());
	setting.push_back(button1);
	Button button2(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 3.0f * g_height / 10.0f), "Set difficulty", MainMenu::getFont());
	setting.push_back(button2);
	Button button3(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 4.0f * g_height / 10.0f), "Turn music on", MainMenu::getFont());
	setting.push_back(button3);
	Button button4(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 5.0f * g_height / 10.0f), "Music volume", MainMenu::getFont());
	setting.push_back(button4);
	Button button5(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 6.0f * g_height / 10.0f), "Turn sfx on", MainMenu::getFont());
	setting.push_back(button5);
	Button button6(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 7.0f * g_height / 10.0f), "Sfx volume", MainMenu::getFont());
	setting.push_back(button6);
	Button button7(sf::Vector2f(g_width / 5.5f, g_height / 10.0f - 3.0f), sf::Vector2f(g_width / 2.0f, 8.0f * g_height / 10.0f), "Back", MainMenu::getFont());
	setting.push_back(button7);

	music_volume_background = sf::RectangleShape(sf::Vector2f(g_width / 6.0f, 8.0f));
	music_volume_background.setOutlineThickness(2.0f);
	music_volume_background.setOutlineColor(sf::Color::Black);
	music_volume_background.setOrigin(music_volume_background.getLocalBounds().width / 2.0f, music_volume_background.getLocalBounds().height / 2.0f);
	music_volume_background.setPosition(sf::Vector2f(11.5f * g_width / 16.0f, 5.05f * g_height / 10.0f));
	
	float gap = music_volume_background.getGlobalBounds().left - button4.getOuterBox().getGlobalBounds().left - button4.getOuterBox().getLocalBounds().width;

	music_volume_background_helper = sf::RectangleShape(sf::Vector2f(gap + music_volume_background.getLocalBounds().width + gap, g_height / 10.0f - 3.0f));
	music_volume_background_helper.setOrigin(music_volume_background_helper.getLocalBounds().width / 2.0f, music_volume_background_helper.getLocalBounds().height / 2.0f);
	music_volume_background_helper.setPosition(music_volume_background.getPosition().x, 5.0f * g_height / 10.0f);

	music_volume_adjuster = sf::RectangleShape(sf::Vector2f(g_width / 400.0f, g_height / 25.0f));
	music_volume_adjuster.setOutlineThickness(2.0f);
	music_volume_adjuster.setOutlineColor(sf::Color::Black);
	music_volume_adjuster.setOrigin(music_volume_adjuster.getLocalBounds().width / 2.0f, music_volume_adjuster.getLocalBounds().height / 2.0f);
	music_volume_adjuster.setPosition(music_volume_background.getGlobalBounds().left + static_cast<float>(music_volume) * music_volume_background.getLocalBounds().width / 100.f, 5.05f * g_height / 10.0f);


	Button button8(sf::Vector2f(g_width / 100.0f, g_height / 10.0f), sf::Vector2f(music_volume_background.getGlobalBounds().left + music_volume_background.getGlobalBounds().width + gap + g_width / 200.0f, 
		5.05f * g_height / 10.0f), std::to_string(music_volume), MainMenu::getFont());
	setting.push_back(button8);

	sfx_volume_background = sf::RectangleShape(sf::Vector2f(g_width / 6.0f, 8.0f));
	sfx_volume_background.setOutlineThickness(2.0f);
	sfx_volume_background.setOutlineColor(sf::Color::Black);
	sfx_volume_background.setOrigin(sfx_volume_background.getLocalBounds().width / 2.0f, sfx_volume_background.getLocalBounds().height / 2.0f);
	sfx_volume_background.setPosition(sf::Vector2f(11.5f * g_width / 16.0f, 7.05f * g_height / 10.0f));

	gap = sfx_volume_background.getGlobalBounds().left - button6.getOuterBox().getGlobalBounds().left - button6.getOuterBox().getLocalBounds().width;

	sfx_volume_background_helper = sf::RectangleShape(sf::Vector2f(gap + sfx_volume_background.getLocalBounds().width + gap, g_height / 10.0f - 3.0f));
	sfx_volume_background_helper.setOrigin(sfx_volume_background_helper.getLocalBounds().width / 2.0f, sfx_volume_background_helper.getLocalBounds().height / 2.0f);
	sfx_volume_background_helper.setPosition(sfx_volume_background.getPosition().x, 7.0f * g_height / 10.0f);

	sfx_volume_adjuster = sf::RectangleShape(sf::Vector2f(g_width / 400.0f, g_height / 25.0f));
	sfx_volume_adjuster.setOutlineThickness(2.0f);
	sfx_volume_adjuster.setOutlineColor(sf::Color::Black);
	sfx_volume_adjuster.setOrigin(sfx_volume_adjuster.getLocalBounds().width / 2.0f, sfx_volume_adjuster.getLocalBounds().height / 2.0f);
	sfx_volume_adjuster.setPosition(sfx_volume_background.getGlobalBounds().left + static_cast<float>(sfx_volume) * sfx_volume_background.getLocalBounds().width / 100.f, 7.05f * g_height / 10.0f);

	Button button9(sf::Vector2f(g_width / 100.0f, g_height / 10.0f), sf::Vector2f(sfx_volume_background.getGlobalBounds().left + sfx_volume_background.getGlobalBounds().width + gap + g_width / 200.0f,
		7.05f * g_height / 10.0f), std::to_string(sfx_volume), MainMenu::getFont());
	setting.push_back(button9);
}

int Settings::Run(sf::RenderWindow & render_window, std::vector<sf::Sound> & sounds)
{
	sf::Event event;
	sf::Vector2i mouse_position;

	// correct the sfx_volume_adjuster in case the sound was muted in another screen
	sfx_volume_adjuster.setPosition(sfx_volume_background.getGlobalBounds().left + static_cast<float>(sfx_volume) * sfx_volume_background.getLocalBounds().width / 100.0f, 7.05f * g_height / 10.0f);
	setting[8].setText(std::to_string(sfx_volume));

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

			if (event.type == sf::Event::EventType::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					setting_selected = (setting_selected > 0) ? --setting_selected : 6;
					sounds[0].play();
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					setting_selected = (setting_selected < 6) ? ++setting_selected : 0;
					sounds[0].play();
				}

				if (event.key.code == sf::Keyboard::Right)
				{
					if (setting_selected == 3) // music volume selected
					{
						music_volume = (music_volume < 100) ? ++music_volume : 100;
						music_volume_adjuster.setPosition(static_cast<float>(music_volume) * music_volume_background.getGlobalBounds().width / 100.0f + music_volume_background.getGlobalBounds().left, 5.05f * g_height / 10.0f);
						setting[7].setText(std::to_string(music_volume));
					}

					if (setting_selected == 5) // sfx volume selected
					{
						if (muted_sfx)
						{
							sfx_volume = muted_sfx_volume;
							muted_sfx = false;
						}
						sfx_volume = (sfx_volume < 100) ? ++sfx_volume : 100;
						sfx_volume_adjuster.setPosition(static_cast<float>(sfx_volume) * sfx_volume_background.getGlobalBounds().width / 100.0f + sfx_volume_background.getGlobalBounds().left, 7.05f * g_height / 10.0f);
						setting[8].setText(std::to_string(sfx_volume));
						
						for (int i = 0; i < sounds.size(); ++i)
							sounds[i].setVolume(sfx_volume);
						
						if (sfx_volume < 100)
							sounds[0].play();
					}
				}

				if (event.key.code == sf::Keyboard::Left)
				{
					if (setting_selected == 3) // music volume selected
					{
						music_volume = (music_volume > 0) ? --music_volume : 0;
						music_volume_adjuster.setPosition(static_cast<float>(music_volume) * music_volume_background.getGlobalBounds().width / 100.0f + music_volume_background.getGlobalBounds().left, 5.05f * g_height / 10.0f);
						setting[7].setText(std::to_string(music_volume));
					}

					if (setting_selected == 5) // sfx volume selected
					{
						if (muted_sfx)
						{
							sfx_volume = muted_sfx_volume;
							muted_sfx = false;
						}
						sfx_volume = (sfx_volume > 0) ? --sfx_volume : 0;
						sfx_volume_adjuster.setPosition(static_cast<float>(sfx_volume) * sfx_volume_background.getGlobalBounds().width / 100.0f + sfx_volume_background.getGlobalBounds().left, 7.05f * g_height / 10.0f);
						setting[8].setText(std::to_string(sfx_volume));

						for (int i = 0; i < sounds.size(); ++i)
							sounds[i].setVolume(sfx_volume);

						if (sfx_volume > 0)
							sounds[0].play();
					}
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					sounds[2].play();
					return 0;	// open menu
				}

				if (event.key.code == sf::Keyboard::Return)
				{
					switch (setting_selected)
					{
					case 0: 
						sounds[1].play();
						return 3;	// open controls 
					case 6: sounds[2].play();
						return 0;	// open menu
					}
				}

				if (event.key.code == sf::Keyboard::M)
				{
					Mute(sounds);
				}
			}

			if (event.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (setting[0].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					return 3; // open controls screen

				if (setting[6].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					return 0; // open main menu
			}
		}

		mouse_position = sf::Mouse::getPosition(render_window);
		cursor.setPosition(mouse_position);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (music_volume_background_helper.getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
			{
				if (mouse_position.x < music_volume_background.getGlobalBounds().left)
					music_volume_adjuster.setPosition(music_volume_background.getGlobalBounds().left, 5.05f * g_height / 10.0f);
				else if (mouse_position.x > music_volume_background.getGlobalBounds().left + music_volume_background.getGlobalBounds().width)
					music_volume_adjuster.setPosition(music_volume_background.getGlobalBounds().left + music_volume_background.getGlobalBounds().width, 5.05f * g_height / 10.0f);
				else
					music_volume_adjuster.setPosition(mouse_position.x, 5.05f * g_height / 10.0f);
				music_volume = 100.0f * (music_volume_adjuster.getPosition().x - static_cast<int>(music_volume_background.getGlobalBounds().left)) / music_volume_background.getLocalBounds().width;
				setting[7].setText(std::to_string(music_volume));
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (sfx_volume_background_helper.getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
			{
				if (mouse_position.x < sfx_volume_background.getGlobalBounds().left)
					sfx_volume_adjuster.setPosition(sfx_volume_background.getGlobalBounds().left, 7.05f * g_height / 10.0f);
				else if (mouse_position.x > sfx_volume_background.getGlobalBounds().left + sfx_volume_background.getGlobalBounds().width)
					sfx_volume_adjuster.setPosition(sfx_volume_background.getGlobalBounds().left + sfx_volume_background.getGlobalBounds().width, 7.05f * g_height / 10.0f);
				else
					sfx_volume_adjuster.setPosition(mouse_position.x, 7.05f * g_height / 10.0f);
				sfx_volume = 100.0f * (sfx_volume_adjuster.getPosition().x - static_cast<int>(sfx_volume_background.getGlobalBounds().left)) / sfx_volume_background.getLocalBounds().width;
				setting[8].setText(std::to_string(sfx_volume));

				for (int i = 0; i < sounds.size(); ++i)
					sounds[i].setVolume(sfx_volume);

				muted_sfx = false;
			}
		}

		for (int i = 0; i < 7; ++i)
		{
			setting[i].setOutlineColor(sf::Color::White);
			if (setting[i].getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))) && has_focus)
			{
				if (i != setting_selected)
					sounds[0].play();
				setting_selected = i;
			}
		}

		setting[setting_selected].setOutlineColor(sf::Color::Black);

		render_window.clear(sf::Color::White);
		for (int i = 0; i < 9; ++i)
			render_window.draw(setting[i]);
		render_window.draw(music_volume_background);
		render_window.draw(music_volume_adjuster);
		render_window.draw(sfx_volume_background);
		render_window.draw(sfx_volume_adjuster);
		if (cursor.isVisible())
			render_window.draw(cursor);
		render_window.display();
	}

	return -1;
}

void Settings::Mute(std::vector<sf::Sound> & sounds)
{
	Screen::Mute(sounds);
	sfx_volume_adjuster.setPosition(sfx_volume_background.getGlobalBounds().left + static_cast<float>(sfx_volume) * sfx_volume_background.getLocalBounds().width / 100.0f, 7.05f * g_height / 10.0f);
	setting[8].setText(std::to_string(sfx_volume));
}