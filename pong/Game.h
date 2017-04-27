#pragma once
#include "Button.h"
#include "Ball.h"
#include "Bracket.h"
#include "Cursor.h"
#include "Screen.h"
#include "MainMenu.h"
#include <cmath>
#include <random>
#include <iostream>

class Bracket;
class Ball;

class Game : public Screen
{
private:
	std::mt19937 engine;
	std::uniform_int_distribution<int> dist;
	enum direction {up, down};
	enum collision_point {left_wall, left_bracket_top, left_bracket_mid, left_bracket_bot, top_wall, bot_wall, right_bracket_top, right_bracket_mid, right_bracket_bot, right_wall};
	Cursor cursor;
	sf::Clock clock;
	sf::Clock move_clock;
	float frame_time;
	float accumulator;
	float dt;
	static bool game_paused;
	bool game_unpausing;
	static int number_of_players;
	bool left_player_name_inserted;
	bool left_player_name_inserting;
	bool right_player_name_inserted;
	bool right_player_name_inserting;
	Button player_left;
	Button player_right;
	Button paused_screen;
	int player_left_score;
	int player_right_score;
	Button score;
	Bracket left_bracket;
	Bracket right_bracket;
	Ball ball;
public:
	Game();
	virtual int Run(sf::RenderWindow &, std::vector<sf::Sound> &);
	void move(Bracket &, direction, float time);
	void update(Ball &);
	static void setNumberOfPlayer(int nop);
	static bool isPaused();
};

