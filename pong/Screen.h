// Screen class, every other class that prints to the screen must inherit from this one
#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <vector>

class Screen
{
public:
	virtual int Run(sf::RenderWindow &, std::vector<sf::Sound> &) = 0;
	virtual void Mute(std::vector<sf::Sound> &);
};

void LoadSounds(std::vector<sf::SoundBuffer> &, std::vector<sf::Sound> &);
