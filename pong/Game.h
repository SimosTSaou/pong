#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Button.h"
#include "Ball.h"
#include "Bracket.h"
#include "Cursor.h"
#include "Screen.h"
#include "MainMenu.h"
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
	static bool game_won;
	static bool game_restart;
	static bool game_unpausing;
	static int number_of_players;
	bool left_player_name_inserted;
	bool left_player_name_inserting;
	bool right_player_name_inserted;
	bool right_player_name_inserting;
	Button player_left;
	Button player_right;
	Button paused_screen;
	Button win_screen;
	int player_left_score;
	int player_right_score;
	Button score;
	Bracket left_bracket;
	Bracket right_bracket;
	Ball ball;
	void move(Bracket &, direction, float time);
	void update(Ball &);
public:
	Game();
	virtual int Run(sf::RenderWindow &, std::vector<sf::Sound> &);
	static void setNumberOfPlayer(int nop);
	static bool isPaused();
};

