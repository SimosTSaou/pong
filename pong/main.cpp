#include <iostream>
#include "Screens.h"
#include "SFML\Audio.hpp"
#include <vector>

extern int g_width;
extern int g_height;

int main()
{
	sf::RenderWindow render_window(sf::VideoMode(g_width, g_height), "Awesome Demo Title", sf::Style::Close);
	render_window.setFramerateLimit(60);

	std::vector<sf::SoundBuffer> sound_buffers;
	std::vector<sf::Sound> sounds;
	LoadSounds(sound_buffers, sounds);
	
	std::vector<Screen *> screen;
	MainMenu mm;
	screen.push_back(&mm);
	Settings st;
	screen.push_back(&st);
	Game gm;
	screen.push_back(&gm);
	Controls ct;
	screen.push_back(&ct);

	int stop = 0;

	while (stop >= 0)
	{
		stop = screen[stop]->Run(render_window, sounds);
	}

	

	std::cout << "DONE!\n";
	return 0;
}













/*

if (event.type == sf::Event::EventType::KeyPressed)
{
// Up and Down volume control
if (event.key.code == sf::Keyboard::Down)
music.setVolume(music.getVolume() - 10.0f);
if (event.key.code == sf::Keyboard::Up)
music.setVolume(music.getVolume() + 10.0f);

// Left and Right to control tracking position
if (event.key.code == sf::Keyboard::Right)
{
auto new_pos = music.getPlayingOffset() + sf::seconds(5.0f);
music.setPlayingOffset(sf::Time(new_pos));
}
if (event.key.code == sf::Keyboard::Left)
{
auto new_pos = music.getPlayingOffset() - sf::seconds(5.0f);
if (new_pos.asSeconds() <= 0.0f) new_pos = sf::seconds(0.0f);
music.setPlayingOffset(sf::Time(new_pos));
}

// Pause and Play, Stop and Start
if (event.key.code == sf::Keyboard::P)
{
if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
{
if (music.getStatus() == sf::SoundSource::Status::Playing)
music.stop();
else if (music.getStatus() == sf::SoundSource::Status::Stopped)
music.play();
}
else
{
if (music.getStatus() == sf::SoundSource::Status::Playing)
music.pause();
else if (music.getStatus() == sf::SoundSource::Status::Paused)
music.play();
}
}

std::cout << "Current volume is :" << music.getVolume() << " position is :" << music.getPlayingOffset().asSeconds() << std::endl;
}

*/