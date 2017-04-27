#include "Game.h"
#include <SFML\Graphics.hpp>

extern int g_width;
extern int g_height;
extern int music_volume;
extern int sfx_volume;
extern int muted_sfx_volume;
extern bool muted_sfx;
extern bool has_focus;

bool Game::game_paused = true;
int Game::number_of_players = 2;

Game::Game()
{
	game_unpausing = false;

	left_player_name_inserted = false;
	left_player_name_inserting = false;
	right_player_name_inserted = false;
	right_player_name_inserting = false;

	player_left_score = 0;
	player_right_score = 0;

	player_left = Button(sf::Vector2f(2.0f * g_width / 5.0f - 2.5f, g_height / 12.0f - 1.5f), sf::Vector2f(g_width / 5.0f, g_height / 24.0f), "Click to enter name", MainMenu::getFont());
	player_left.setOutlineColor(sf::Color::Black);

	player_right = Button(sf::Vector2f(2.0f * g_width / 5.0f - 2.5f, g_height / 12.0f - 1.5f), sf::Vector2f(4.0f * g_width / 5.0f, g_height / 24.0f), "Click to enter name", MainMenu::getFont());
	player_right.setOutlineColor(sf::Color::Black);

	score = Button(sf::Vector2f(g_width / 5.0f - 2.5f, g_height / 12.0f - 1.5f), sf::Vector2f(g_width / 2.0f, g_height / 24.0f), "0    -    0", MainMenu::getFont());
	score.setOutlineColor(sf::Color::Black);

	paused_screen = Button(sf::Vector2f(g_width / 3.0f, g_height / 10.0f), sf::Vector2f(g_width / 2.0f, g_height / 2.0f), "Game Paused", MainMenu::getFont());
	paused_screen.setOutlineColor(sf::Color::Black);

	left_bracket = Bracket(sf::Vector2f(g_width / 40.0f, g_height / 6.0f), sf::Vector2f(g_width / 25.0f - g_width / 80.0f, g_height / 2.0f));
	right_bracket = Bracket(sf::Vector2f(g_width / 40.0f, g_height / 6.0f), sf::Vector2f(24.0f * g_width / 25.0f + g_width / 80.0f, g_height / 2.0f));

	dt = 1000.0f / 60.0f;
	accumulator = 0.0f;

	std::random_device rd{};
	engine.seed(rd());
	dist = std::uniform_int_distribution<int>{ 0, 359 };

	ball = Ball(sf::Vector2f(g_width / 200.8f, g_height / 124.0f), sf::Vector2f(g_width / 2.0f, g_height / 2.0f));
	ball.angle = dist(engine);
	while ((ball.angle > 60 && ball.angle < 120) || (ball.angle > 240 && ball.angle < 300))
		ball.angle = dist(engine);

	if (ball.angle >= 0.0 && ball.angle <= 60.0)
	{
		ball.direction = sf::Vector2f(cos(ball.angle), sin(ball.angle));
		std::cout << "CHECK1\n";
	}
	else if (ball.angle >= 120.0 && ball.angle <= 180.0)
	{
		ball.direction = sf::Vector2f(-cos(180.0 - ball.angle), sin(180.0 - ball.angle));
		std::cout << "CHECK2\n";
	}
	else if (ball.angle > 180.0 && ball.angle <= 240.0)
	{
		ball.direction = sf::Vector2f(-cos(ball.angle) - 180.0, -sin(ball.angle) - 180.0);
		std::cout << "CHECK3\n";
	}
	else
	{
		ball.direction = sf::Vector2f(cos(360.0 - ball.angle), -sin(360.0 - ball.angle));
		std::cout << "CHECK4\n";
	}
}

void Game::move(Bracket & bracket, Game::direction dir, float time)
{
	if (dir == up)
	{
		if (bracket.getOuterBox().getGlobalBounds().top > player_left.getOuterBox().getGlobalBounds().top + player_left.getOuterBox().getLocalBounds().height + bracket.speed * time)
			bracket.getOuterBox().setPosition(bracket.getOuterBox().getPosition().x, bracket.getOuterBox().getPosition().y - bracket.speed * time);
		else
			bracket.getOuterBox().setPosition(bracket.getOuterBox().getPosition().x, 
				player_left.getOuterBox().getGlobalBounds().top + player_left.getOuterBox().getLocalBounds().height + bracket.getOuterBox().getGlobalBounds().height / 2.0f);
	}
	else if (dir == down)
	{
		if (bracket.getOuterBox().getPosition().y + bracket.getOuterBox().getGlobalBounds().height / 2.0f < g_height - bracket.speed * time)
			bracket.getOuterBox().setPosition(bracket.getOuterBox().getPosition().x, bracket.getOuterBox().getPosition().y + bracket.speed * time);
		else
			bracket.getOuterBox().setPosition(bracket.getOuterBox().getPosition().x, g_height - bracket.getOuterBox().getGlobalBounds().height / 2.0f);
	}
}

int Game::Run(sf::RenderWindow & render_window, std::vector<sf::Sound> & sounds)
{
	sf::Event event;

	move_clock.restart();

	std::cout << ball.angle << '\n';
	std::cout << ball.direction.x << ", " << ball.direction.y << '\n';
	std::cout << cos(ball.angle) << '\n';

	while (render_window.isOpen())
	{
		frame_time = move_clock.restart().asMilliseconds();
		accumulator += frame_time;

		while (accumulator >= dt)
		{
			while (render_window.pollEvent(event))
			{
				if (event.type == sf::Event::EventType::Closed)
					render_window.close();

				// focus event
				if (event.type == sf::Event::LostFocus)
				{
					has_focus = false;
					game_paused = true;
					game_unpausing = false;
					paused_screen.setText("Game Paused");
					if (left_player_name_inserting)
					{
						left_player_name_inserting = false;
						player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 1));
						if (player_left.getText().getSize() == 0)
						{
							player_left.setText("Click to enter name");
							left_player_name_inserted = false;
						}
						else
							left_player_name_inserted = true;
					}
				}
				else if (event.type == sf::Event::GainedFocus)
					has_focus = true;

				// escape button
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				{
					if (left_player_name_inserting)
					{
						left_player_name_inserting = false;
						player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 1));
						if (player_left.getText().getSize() == 0)
						{
							player_left.setText("Click to enter name");
							left_player_name_inserted = false;
						}
						else
							left_player_name_inserted = true;
					}
					else if (right_player_name_inserting)
					{
						player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 1));
						if (player_right.getText().getSize() == 0)
						{
							player_right.setText("Click to enter name");
							right_player_name_inserted = false;
						}
						else
							right_player_name_inserted = true;
						right_player_name_inserting = false;
					}
					paused_screen.setText("Game Paused");
					game_unpausing = false;
					game_paused = true;
					sounds[2].play();
					return 0;
				}

				// name inserting
				if (left_player_name_inserting)
				{
					if (event.type == sf::Event::TextEntered)
					{
						if (event.text.unicode >= 32 && event.text.unicode <= 126)
						{
							if (player_left.getText().getSize() < 20)
								player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 1) + static_cast<char>(event.text.unicode) + "_");
						}
						else if (event.text.unicode == 8 && player_left.getText().getSize() > 1)
							player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 2) + "_");
						else if (event.text.unicode == 13)
						{
							player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 1));
							if (player_left.getText().getSize() == 0)
							{
								player_left.setText("Click to enter name");
								left_player_name_inserted = false;
							}
							else
								left_player_name_inserted = true;
							left_player_name_inserting = false;
						}
					}
				}
				else if (right_player_name_inserting)
				{
					if (event.type == sf::Event::TextEntered)
					{
						if (event.text.unicode >= 32 && event.text.unicode <= 126)
						{
							if (player_right.getText().getSize() < 20)
								player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 1) + static_cast<char>(event.text.unicode) + "_");
						}
						else if (event.text.unicode == 8 && player_right.getText().getSize() > 1)
							player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 2) + "_");
						else if (event.text.unicode == 13)
						{
							player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 1));
							if (player_right.getText().getSize() == 0)
							{
								player_right.setText("Click to enter name");
								right_player_name_inserted = false;
							}
							else
								right_player_name_inserted = true;
							right_player_name_inserting = false;
						}
					}
				}
				else
				{
					if (event.type == sf::Event::KeyPressed)
					{
						if (event.key.code == sf::Keyboard::M)
							Mute(sounds);

						if (event.key.code == sf::Keyboard::P)
						{
							if (game_paused)
							{
								paused_screen.setText("Game starts in 5...");
								game_paused = false;
								game_unpausing = true;
								clock.restart();
							}
							else
							{
								paused_screen.setText("Game Paused");
								game_paused = true;
								game_unpausing = false;
							}
						}
					}
				}

				// mouse buttons pressed that trigger name inserting or pause/unpause
				if (event.type == sf::Event::EventType::MouseButtonPressed)
				{
					if (player_left.getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					{
						game_paused = true;
						game_unpausing = false;
						paused_screen.setText("Game Paused");
						if (!left_player_name_inserted && !left_player_name_inserting)
							player_left.setText("");
						if (!left_player_name_inserting)
							player_left.setText(player_left.getText() + "_");
						left_player_name_inserting = true;

						if (right_player_name_inserting)
						{
							player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 1));
							if (player_right.getText().getSize() == 0)
							{
								player_right.setText("Click to enter name");
								right_player_name_inserted = false;
							}
							else
								right_player_name_inserted = true;
							right_player_name_inserting = false;
						}
					}
					else if (player_right.getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))))
					{
						game_paused = true;
						game_unpausing = false;
						paused_screen.setText("Game Paused");
						if (!right_player_name_inserted && !right_player_name_inserting)
							player_right.setText("");
						if (!right_player_name_inserting)
							player_right.setText(player_right.getText() + "_");
						right_player_name_inserting = true;

						if (left_player_name_inserting)
						{
							player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 1));
							if (player_left.getText().getSize() == 0)
							{
								player_left.setText("Click to enter name");
								left_player_name_inserted = false;
							}
							else
								left_player_name_inserted = true;
							left_player_name_inserting = false;
						}
					}
					else
					{
						if (left_player_name_inserting)
						{
							player_left.setText(player_left.getText().substring(0, player_left.getText().getSize() - 1));
							if (player_left.getText().getSize() == 0)
							{
								player_left.setText("Click to enter name");
								left_player_name_inserted = false;
							}
							else
								left_player_name_inserted = true;
							left_player_name_inserting = false;
						}

						if (right_player_name_inserting)
						{
							player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 1));
							if (player_right.getText().getSize() == 0)
							{
								player_right.setText("Click to enter name");
								right_player_name_inserted = false;
							}
							else
								right_player_name_inserted = true;
							right_player_name_inserting = false;
						}
					}



					if (paused_screen.getOuterBox().getGlobalBounds().contains(render_window.mapPixelToCoords(sf::Mouse::getPosition(render_window))) && game_paused)
					{
						paused_screen.setText("Game starts in 5...");
						game_paused = false;
						game_unpausing = true;
						clock.restart();
					}
				}
			}

			if (!(game_paused || game_unpausing))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					move(left_bracket, up, dt);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					move(left_bracket, down, dt);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					move(right_bracket, up, dt);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					move(right_bracket, down, dt);

				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x + ball.direction.x * ball.speed * dt, ball.getOuterBox().getPosition().y - ball.direction.y * ball.speed * dt);
			}

			accumulator -= dt;
		}

		if (game_unpausing)
		{
			if (clock.getElapsedTime().asMilliseconds() <= 5000)
				paused_screen.setText("Game starts in " + std::to_string(static_cast<int>((5000 - clock.getElapsedTime().asMilliseconds()) / 1000)) + "...");
			else if (clock.getElapsedTime().asMilliseconds() <= 6000)
				paused_screen.setText("START");
			else
			{
				game_unpausing = false;
				move_clock.restart();
			}
		}

		cursor.setPosition(sf::Mouse::getPosition(render_window));

		render_window.clear(sf::Color::White);
		render_window.draw(ball);
		render_window.draw(left_bracket);
		render_window.draw(right_bracket);
		render_window.draw(player_left);
		render_window.draw(player_right);
		render_window.draw(score);
		if (game_paused || game_unpausing)
			render_window.draw(paused_screen);
		render_window.draw(cursor);
		render_window.display();
	}

	return -1;
}

void Game::setNumberOfPlayer(int nop)
{
	number_of_players = nop;
}

bool Game::isPaused()
{
	return game_paused;
}