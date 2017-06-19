#include "Game.h"
#include <SFML\Graphics.hpp>

extern int g_width;
extern int g_height;
extern int music_volume;
extern int sfx_volume;
extern int muted_sfx_volume;
extern bool muted_sfx;
extern bool has_focus;
extern bool playing;

bool Game::game_paused = false;
bool Game::game_won = false;
bool Game::game_restart = false;
bool Game::game_unpausing = false;
int Game::number_of_players = 2;

Game::Game()
{
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

	win_screen = Button(sf::Vector2f(g_width / 3.0f, g_height / 10.0f), sf::Vector2f(g_width / 2.0f, paused_screen.getOuterBox().getPosition().y + g_height / 10.0f), "R (Restart) / Q (Quit)", MainMenu::getFont());
	win_screen.setOutlineColor(sf::Color::Black);

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

	ball.angle = ball.angle * M_PI / 180.0;
	
	if (ball.angle >= 0.0 && ball.angle <= 60.0)
		ball.direction = sf::Vector2f(cos(ball.angle), sin(ball.angle));
	else if (ball.angle >= 120.0 && ball.angle <= 180.0)
		ball.direction = sf::Vector2f(-cos(180.0 - ball.angle), sin(180.0 - ball.angle));
	else if (ball.angle > 180.0 && ball.angle <= 240.0)
		ball.direction = sf::Vector2f(-cos(ball.angle) - 180.0, -sin(ball.angle) - 180.0);
	else
		ball.direction = sf::Vector2f(cos(360.0 - ball.angle), -sin(360.0 - ball.angle));
}

void Game::move(Bracket & bracket, Game::direction dir, float time)
{
	if (dir == up)
	{
		if (bracket.getOuterBox().getGlobalBounds().top > player_left.getOuterBox().getLocalBounds().height + bracket.speed * time)
			bracket.getOuterBox().move(0, -bracket.speed * time);
		else
			bracket.getOuterBox().setPosition(bracket.getOuterBox().getPosition().x, player_left.getOuterBox().getLocalBounds().height + bracket.getOuterBox().getGlobalBounds().height / 2.0f);
	}
	else if (dir == down)
	{
		if (bracket.getOuterBox().getPosition().y + bracket.getOuterBox().getGlobalBounds().height / 2.0f < g_height - bracket.speed * time)
			bracket.getOuterBox().move(0, bracket.speed * time);
		else
			bracket.getOuterBox().setPosition(bracket.getOuterBox().getPosition().x, g_height - bracket.getOuterBox().getGlobalBounds().height / 2.0f);
	}
}

void Game::update(Ball & ball)
{
	double intersection_point_x;

	// top wall
	if (ball.getOuterBox().getGlobalBounds().top <= player_left.getOuterBox().getGlobalBounds().height)
	{
		ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, player_left.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
		ball.direction.y = -ball.direction.y;
	}
	// bottom wall
	else if (ball.getOuterBox().getPosition().y + ball.getOuterBox().getGlobalBounds().height / 2.0f >= g_height)
	{
		ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, g_height - ball.getOuterBox().getGlobalBounds().height / 2.0f);
		ball.direction.y = -ball.direction.y;
	}
	// left bracket
	else if (ball.getOuterBox().getGlobalBounds().intersects(left_bracket.getOuterBox().getGlobalBounds()))
	{
		ball.speed = (ball.speed < 400) ? ball.speed + ball.speed / 20.0f : ball.speed;
		if (ball.direction.y > 0) // ball moving downwards
		{
			if (ball.direction.x > 0)
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top + left_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else if (ball.direction.x < 0 && (ball.getOuterBox().getGlobalBounds().top + ball.getOuterBox().getGlobalBounds().height > left_bracket.getOuterBox().getGlobalBounds().top + left_bracket.getOuterBox().getGlobalBounds().height))
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top + left_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else
			{
				double left_bot_corner_x = ball.getOuterBox().getGlobalBounds().left;
				double left_bot_corner_y = ball.getOuterBox().getGlobalBounds().top + ball.getOuterBox().getGlobalBounds().height;
				double b = left_bot_corner_y - ball.direction.y / ball.direction.x * left_bot_corner_x;

				intersection_point_x = ball.direction.x * (left_bracket.getOuterBox().getGlobalBounds().top - b) / ball.direction.y;
				if (intersection_point_x <= left_bracket.getOuterBox().getGlobalBounds().left + left_bracket.getOuterBox().getGlobalBounds().width / 2.0f) // top side, left part
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.y = -ball.direction.y;
				}
				else if (intersection_point_x < left_bracket.getOuterBox().getGlobalBounds().left + left_bracket.getOuterBox().getGlobalBounds().width) // top side, right part
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.x = -ball.direction.x;
					ball.direction.y = -ball.direction.y;
				}
				else // right side
				{
					ball.getOuterBox().setPosition(left_bracket.getOuterBox().getGlobalBounds().left + left_bracket.getOuterBox().getGlobalBounds().width + ball.getOuterBox().getGlobalBounds().width / 2.0f, ball.getOuterBox().getPosition().y);
					if (ball.getOuterBox().getPosition().y == left_bracket.getOuterBox().getPosition().y) // collision right in the middle
						ball.direction.y = 0;
					else if (ball.getOuterBox().getPosition().y > left_bracket.getOuterBox().getPosition().y)
					{
						if (ball.getOuterBox().getPosition().y < left_bracket.getOuterBox().getPosition().y + left_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 2/4 and 3/4
							ball.direction.y = ball.direction.y - (1.0f - (ball.getOuterBox().getPosition().y - left_bracket.getOuterBox().getPosition().y) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 3/4 and 4/4
							ball.direction.y = ball.direction.y + ((ball.getOuterBox().getPosition().y - (left_bracket.getOuterBox().getPosition().y + left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					else
					{
						if (ball.getOuterBox().getPosition().y > left_bracket.getOuterBox().getPosition().y - left_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 1/4 and 2/4
							ball.direction.y = -ball.direction.y + (1.0f - (left_bracket.getOuterBox().getPosition().y - ball.getOuterBox().getPosition().y) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 0/4 and 1/4
							ball.direction.y = -ball.direction.y - ((left_bracket.getOuterBox().getPosition().y - left_bracket.getOuterBox().getGlobalBounds().height / 4.0f - ball.getOuterBox().getPosition().y) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					ball.direction.x = sqrt(1 - ball.direction.y * ball.direction.y);
				}
			}
		}
		else if (ball.direction.y < 0) // ball moving upwards
		{
			if (ball.direction.x > 0)
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else if (ball.direction.x < 0 && (ball.getOuterBox().getGlobalBounds().top < left_bracket.getOuterBox().getGlobalBounds().top))
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else
			{
				double left_top_corner_x = ball.getOuterBox().getGlobalBounds().left;
				double left_top_corner_y = ball.getOuterBox().getGlobalBounds().top;
				double b = left_top_corner_y - ball.direction.y / ball.direction.x * left_top_corner_x;

				intersection_point_x = ball.direction.x * (left_bracket.getOuterBox().getGlobalBounds().top + left_bracket.getOuterBox().getGlobalBounds().height - b) / ball.direction.y;
				if (intersection_point_x <= left_bracket.getOuterBox().getGlobalBounds().left + left_bracket.getOuterBox().getGlobalBounds().width / 2.0f)
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top + left_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.y = -ball.direction.y;
				}
				else if (intersection_point_x < left_bracket.getOuterBox().getGlobalBounds().left + left_bracket.getOuterBox().getGlobalBounds().width)
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, left_bracket.getOuterBox().getGlobalBounds().top + left_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.x = -ball.direction.x;
					ball.direction.y = -ball.direction.y;
				}
				else // right side
				{
					ball.getOuterBox().setPosition(left_bracket.getOuterBox().getGlobalBounds().left + left_bracket.getOuterBox().getGlobalBounds().width + ball.getOuterBox().getGlobalBounds().width / 2.0f, ball.getOuterBox().getPosition().y);
					if (ball.getOuterBox().getPosition().y == left_bracket.getOuterBox().getPosition().y) // collision right in the middle
						ball.direction.y = 0;
					else if (ball.getOuterBox().getPosition().y > left_bracket.getOuterBox().getPosition().y)
					{
						if (ball.getOuterBox().getPosition().y < left_bracket.getOuterBox().getPosition().y + left_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 2/4 and 3/4
							ball.direction.y = -ball.direction.y - (1.0f - (ball.getOuterBox().getPosition().y - left_bracket.getOuterBox().getPosition().y) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 3/4 and 4/4
							ball.direction.y = -ball.direction.y + ((ball.getOuterBox().getPosition().y - (left_bracket.getOuterBox().getPosition().y + left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					else
					{
						if (ball.getOuterBox().getPosition().y > left_bracket.getOuterBox().getPosition().y - left_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 1/4 and 2/4
							ball.direction.y = ball.direction.y + (1.0f - (left_bracket.getOuterBox().getPosition().y - ball.getOuterBox().getPosition().y) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 0/4 and 1/4
							ball.direction.y = ball.direction.y - ((left_bracket.getOuterBox().getPosition().y - left_bracket.getOuterBox().getGlobalBounds().height / 4.0f - ball.getOuterBox().getPosition().y) / (left_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					ball.direction.x = sqrt(1 - ball.direction.y * ball.direction.y);
				}
			}
		}
		else // ball moving horizontally
		{
			ball.direction.y = ((ball.getOuterBox().getPosition().y - left_bracket.getOuterBox().getPosition().y) / left_bracket.getOuterBox().getGlobalBounds().height / 2.0f) / 2.0f;
			ball.direction.x = sqrt(1.0f - ball.direction.y * ball.direction.y);
		}
	}
	// right bracket
	else if (ball.getOuterBox().getGlobalBounds().intersects(right_bracket.getOuterBox().getGlobalBounds()))
	{
		ball.speed = (ball.speed < 400) ? ball.speed + ball.speed / 20.0f : ball.speed;
		if (ball.direction.y > 0) // ball moving downwards
		{
			if (ball.direction.x < 0)
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top + right_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else if (ball.direction.x > 0 && (ball.getOuterBox().getGlobalBounds().top + ball.getOuterBox().getGlobalBounds().height > right_bracket.getOuterBox().getGlobalBounds().top + right_bracket.getOuterBox().getGlobalBounds().height))
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top + right_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else
			{
				double right_bot_corner_x = ball.getOuterBox().getGlobalBounds().left + ball.getOuterBox().getGlobalBounds().width;
				double right_bot_corner_y = ball.getOuterBox().getGlobalBounds().top + ball.getOuterBox().getGlobalBounds().height;
				double b = right_bot_corner_y - ball.direction.y / ball.direction.x * right_bot_corner_x;

				intersection_point_x = ball.direction.x * (right_bracket.getOuterBox().getGlobalBounds().top - b) / ball.direction.y;
				if (intersection_point_x >= right_bracket.getOuterBox().getGlobalBounds().left + right_bracket.getOuterBox().getGlobalBounds().width / 2.0f) // top side, right part
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.y = -ball.direction.y;
				}
				else if (intersection_point_x > right_bracket.getOuterBox().getGlobalBounds().left) // top side, left part
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.x = -ball.direction.x;
					ball.direction.y = -ball.direction.y;
				}
				else // left side
				{
					ball.getOuterBox().setPosition(right_bracket.getOuterBox().getGlobalBounds().left - ball.getOuterBox().getGlobalBounds().width / 2.0f, ball.getOuterBox().getPosition().y);
					if (ball.getOuterBox().getPosition().y == right_bracket.getOuterBox().getPosition().y) // collision right in the middle
						ball.direction.y = 0;
					else if (ball.getOuterBox().getPosition().y > right_bracket.getOuterBox().getPosition().y) 
					{
						if (ball.getOuterBox().getPosition().y < right_bracket.getOuterBox().getPosition().y + right_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 2/4 and 3/4
							ball.direction.y = ball.direction.y - (1.0f - (ball.getOuterBox().getPosition().y - right_bracket.getOuterBox().getPosition().y) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 3/4 and 4/4
							ball.direction.y = ball.direction.y + ((ball.getOuterBox().getPosition().y - (right_bracket.getOuterBox().getPosition().y + right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					else
					{
						if (ball.getOuterBox().getPosition().y > right_bracket.getOuterBox().getPosition().y - right_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 1/4 and 2/4
							ball.direction.y = -ball.direction.y + (1.0f - (right_bracket.getOuterBox().getPosition().y - ball.getOuterBox().getPosition().y) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 0/4 and 1/4
							ball.direction.y = -ball.direction.y - ((right_bracket.getOuterBox().getPosition().y - right_bracket.getOuterBox().getGlobalBounds().height / 4.0f - ball.getOuterBox().getPosition().y) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					ball.direction.x = -sqrt(1 - ball.direction.y * ball.direction.y);
				}
			}
		}
		else if (ball.direction.y < 0) // ball moving upwards
		{
			if (ball.direction.x < 0)
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else if (ball.direction.x > 0 && (ball.getOuterBox().getGlobalBounds().top < right_bracket.getOuterBox().getGlobalBounds().top))
				ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top - ball.getOuterBox().getGlobalBounds().height / 2.0f);
			else
			{
				double right_top_corner_x = ball.getOuterBox().getGlobalBounds().left + ball.getOuterBox().getGlobalBounds().width;
				double right_top_corner_y = ball.getOuterBox().getGlobalBounds().top;
				double b = right_top_corner_y - ball.direction.y / ball.direction.x * right_top_corner_x;

				intersection_point_x = ball.direction.x * (right_bracket.getOuterBox().getGlobalBounds().top + right_bracket.getOuterBox().getGlobalBounds().height - b) / ball.direction.y;
				if (intersection_point_x >= right_bracket.getOuterBox().getGlobalBounds().left + right_bracket.getOuterBox().getGlobalBounds().width / 2.0f) // bottome side right part
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top + right_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.y = -ball.direction.y;
				}
				else if (intersection_point_x > right_bracket.getOuterBox().getGlobalBounds().left) // bottom side left part
				{
					ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x, right_bracket.getOuterBox().getGlobalBounds().top + right_bracket.getOuterBox().getGlobalBounds().height + ball.getOuterBox().getGlobalBounds().height / 2.0f);
					ball.direction.x = -ball.direction.x;
					ball.direction.y = -ball.direction.y;
				}
				else // left side
				{
					ball.getOuterBox().setPosition(right_bracket.getOuterBox().getGlobalBounds().left - ball.getOuterBox().getGlobalBounds().width / 2.0f, ball.getOuterBox().getPosition().y);
					if (ball.getOuterBox().getPosition().y == right_bracket.getOuterBox().getPosition().y) // collision right in the middle
						ball.direction.y = 0;
					else if (ball.getOuterBox().getPosition().y > right_bracket.getOuterBox().getPosition().y)
					{
						if (ball.getOuterBox().getPosition().y < right_bracket.getOuterBox().getPosition().y + right_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 2/4 and 3/4
							ball.direction.y = -ball.direction.y - (1.0f - (ball.getOuterBox().getPosition().y - right_bracket.getOuterBox().getPosition().y) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 3/4 and 4/4
							ball.direction.y = -ball.direction.y + ((ball.getOuterBox().getPosition().y - (right_bracket.getOuterBox().getPosition().y + right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					else
					{
						if (ball.getOuterBox().getPosition().y > right_bracket.getOuterBox().getPosition().y - right_bracket.getOuterBox().getGlobalBounds().height / 4.0f) // collision between 1/4 and 2/4
							ball.direction.y = ball.direction.y + (1.0f - (right_bracket.getOuterBox().getPosition().y - ball.getOuterBox().getPosition().y) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * abs(ball.direction.y);
						else // collision between 0/4 and 1/4
							ball.direction.y = ball.direction.y - ((right_bracket.getOuterBox().getPosition().y - right_bracket.getOuterBox().getGlobalBounds().height / 4.0f - ball.getOuterBox().getPosition().y) / (right_bracket.getOuterBox().getGlobalBounds().height / 4.0f)) * (1.0f - abs(ball.direction.y)) / 2.0f;
					}
					ball.direction.x = -sqrt(1 - ball.direction.y * ball.direction.y);
				}
			}
		}
		else // ball moving horizontally
		{
			ball.direction.y = ((ball.getOuterBox().getPosition().y - right_bracket.getOuterBox().getPosition().y) / right_bracket.getOuterBox().getGlobalBounds().height / 2.0f) / 2.0f;
			ball.direction.x = -sqrt(1.0f - ball.direction.y * ball.direction.y);
		}
	}
	// left wall
	else if (ball.getOuterBox().getGlobalBounds().left <= 0)
	{
		++player_right_score;
		score.setText(std::to_string(player_left_score) + "    -    " + std::to_string(player_right_score));
		if (player_right_score == 1)
		{
			playing = false;
			game_paused = true;
			game_won = true;
			if (right_player_name_inserted)
				paused_screen.setText(player_right.getText() + " won");
			else
				paused_screen.setText("Right player won");
			win_screen.setText("R (Restart) / Q (Quit)");
		}
		else
		{
			ball.getOuterBox().setPosition(sf::Vector2f(g_width / 2.0f, g_height / 2.0f));

			ball.speed = 0.2f;

			ball.angle = dist(engine);
			while ((ball.angle > 60 && ball.angle < 120) || (ball.angle > 240 && ball.angle < 300))
				ball.angle = dist(engine);
			ball.angle = ball.angle * M_PI / 180.0;


			if (ball.angle >= 0.0 && ball.angle <= 60.0)
				ball.direction = sf::Vector2f(cos(ball.angle), sin(ball.angle));
			else if (ball.angle >= 120.0 && ball.angle <= 180.0)
				ball.direction = sf::Vector2f(-cos(180.0 - ball.angle), sin(180.0 - ball.angle));
			else if (ball.angle > 180.0 && ball.angle <= 240.0)
				ball.direction = sf::Vector2f(-cos(ball.angle) - 180.0, -sin(ball.angle) - 180.0);
			else
				ball.direction = sf::Vector2f(cos(360.0 - ball.angle), -sin(360.0 - ball.angle));

			game_unpausing = true;
			clock.restart();
			paused_screen.setText("3");
		}
	}
	// right wall
	else if (ball.getOuterBox().getGlobalBounds().left + ball.getOuterBox().getGlobalBounds().width >= g_width)
	{
		++player_left_score;
		score.setText(std::to_string(player_left_score) + "    -    " + std::to_string(player_right_score));
		if (player_left_score == 1)
		{
			playing = false;
			game_paused = true;
			game_won = true;
			if (left_player_name_inserted == true)
				paused_screen.setText(player_left.getText() + " won");
			else
				paused_screen.setText("Left player won");
			win_screen.setText("R (Restart) / Q (Quit)");
		}
		else
		{
			ball.getOuterBox().setPosition(sf::Vector2f(g_width / 2.0f, g_height / 2.0f));

			ball.speed = 0.2f;

			ball.angle = dist(engine);
			while ((ball.angle > 60 && ball.angle < 120) || (ball.angle > 240 && ball.angle < 300))
				ball.angle = dist(engine);
			ball.angle = ball.angle * M_PI / 180.0;


			if (ball.angle >= 0.0 && ball.angle <= 60.0)
				ball.direction = sf::Vector2f(cos(ball.angle), sin(ball.angle));
			else if (ball.angle >= 120.0 && ball.angle <= 180.0)
				ball.direction = sf::Vector2f(-cos(180.0 - ball.angle), sin(180.0 - ball.angle));
			else if (ball.angle > 180.0 && ball.angle <= 240.0)
				ball.direction = sf::Vector2f(-cos(ball.angle) - 180.0, -sin(ball.angle) - 180.0);
			else
				ball.direction = sf::Vector2f(cos(360.0 - ball.angle), -sin(360.0 - ball.angle));

			game_unpausing = true;
			clock.restart();
			paused_screen.setText("3");
		}
	}
}

int Game::Run(sf::RenderWindow & render_window, std::vector<sf::Sound> & sounds)
{
	sf::Event event;

	// game starts, initialize everything
	if (!playing)
	{
		left_bracket.getOuterBox().setPosition(left_bracket.getOuterBox().getPosition().x, g_height / 2.0f);
		right_bracket.getOuterBox().setPosition(right_bracket.getOuterBox().getPosition().x, g_height / 2.0f);
		
		ball.getOuterBox().setPosition(g_width / 2.0f, g_height / 2.0f);
		ball.speed = 0.2f;

		ball.angle = dist(engine);
		while ((ball.angle > 60 && ball.angle < 120) || (ball.angle > 240 && ball.angle < 300))
			ball.angle = dist(engine);
		ball.angle = ball.angle * M_PI / 180.0;

		if (ball.angle >= 0.0 && ball.angle <= 60.0)
			ball.direction = sf::Vector2f(cos(ball.angle), sin(ball.angle));
		else if (ball.angle >= 120.0 && ball.angle <= 180.0)
			ball.direction = sf::Vector2f(-cos(180.0 - ball.angle), sin(180.0 - ball.angle));
		else if (ball.angle > 180.0 && ball.angle <= 240.0)
			ball.direction = sf::Vector2f(-cos(ball.angle) - 180.0, -sin(ball.angle) - 180.0);
		else
			ball.direction = sf::Vector2f(cos(360.0 - ball.angle), -sin(360.0 - ball.angle));

		player_left_score = 0;
		player_right_score = 0;
		score.setText("0    -    0");
		
		game_won = false;
		game_restart = false;
		game_paused = false;
		game_unpausing = true;
		clock.restart();
		paused_screen.setText("3");

		playing = true;
	}

	move_clock.restart();

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
					if (!(game_won || game_restart))
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
					if (right_player_name_inserting)
					{
						right_player_name_inserting = false;
						player_right.setText(player_right.getText().substring(0, player_right.getText().getSize() - 1));
						if (player_right.getText().getSize() == 0)
						{
							player_right.setText("Click to enter name");
							right_player_name_inserted = false;
						}
						else
							right_player_name_inserted = true;
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
					else if (playing && game_restart)
					{
						game_restart = false;
						game_won = false;
					}
					paused_screen.setText("Game Paused");
					game_unpausing = false;
					game_paused = true;
					sounds[2].play();
					return 0;
				}

				// name inserting and miscellaneous button events
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
							if (!(game_won || game_restart))
							{
								if (game_paused)
								{
									paused_screen.setText("3");
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

						if (event.key.code == sf::Keyboard::R)
						{
							game_unpausing = false;
							game_paused = true;
							game_restart = true;
							game_won = true;
							paused_screen.setText("Restart??");
							win_screen.setText("Yes (Y) / No (N)");
						}

						if (event.key.code == sf::Keyboard::Y)
						{
							if (game_restart)
							{
								playing = false;
								return 2;
							}
						}

						if (event.key.code == sf::Keyboard::N)
						{
							if (game_restart)
							{
								if (playing)
								{
									game_restart = false;
									game_won = false;
									game_paused = false;
									game_unpausing = true;
									clock.restart();
									paused_screen.setText("3");
								}
								else
									return 0;
							}
						}

						if (event.key.code == sf::Keyboard::Q)
						{
							if (game_won && !playing)
								return 0;
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
						if (!game_won)
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
						if (!game_won)
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
						if (!game_won)
						{
							paused_screen.setText("3");
							game_paused = false;
							game_unpausing = true;
							clock.restart();
						}
					}
				}
			}

			if (!(game_paused || game_unpausing))
			{
				//ball.getOuterBox().setPosition(ball.getOuterBox().getPosition().x + ball.direction.x * ball.speed * dt, ball.getOuterBox().getPosition().y + ball.direction.y * ball.speed * dt);
				ball.getOuterBox().move(ball.direction.x * ball.speed * dt, ball.direction.y * ball.speed * dt);
				update(ball);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					move(left_bracket, up, dt);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					move(left_bracket, down, dt);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					move(right_bracket, up, dt);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					move(right_bracket, down, dt);
			}

			accumulator -= dt;
		}

		if (game_unpausing)
		{
			if (clock.getElapsedTime().asMilliseconds() <= 3000)
				paused_screen.setText(std::to_string(static_cast<int>((4000 - clock.getElapsedTime().asMilliseconds()) / 1000)));
			else if (clock.getElapsedTime().asMilliseconds() <= 3500)
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
		if (game_won)
			render_window.draw(win_screen);
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