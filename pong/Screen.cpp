#include "Screen.h"
#include <iostream>

int g_width = 1040;
int g_height = 720;
int music_volume = 50;
int sfx_volume = 0;
int muted_sfx_volume = 0;
bool muted_sfx = false;
int difficulty = 5;
bool has_focus = true;

void Screen::Mute(std::vector<sf::Sound> & sounds)
{
	if (muted_sfx)
	{
		sfx_volume = muted_sfx_volume;
		muted_sfx = false;
	}
	else
	{
		muted_sfx_volume = sfx_volume;
		sfx_volume = 0;
		muted_sfx = true;
	}

	for (unsigned int i = 0; i < sounds.size(); ++i)
		sounds[i].setVolume(static_cast<float>(sfx_volume));
}

void LoadSounds(std::vector<sf::SoundBuffer> & sound_buffers, std::vector<sf::Sound> & sounds)
{
	sound_buffers.resize(3);
	sounds.resize(3);

	if (!sound_buffers[0].loadFromFile("music/menu-select3.wav"))
	{
		std::cout << "Error loading menu select sound\n";
		exit(EXIT_FAILURE);
	}
		
	sounds[0].setBuffer(sound_buffers[0]);
	sounds[0].setVolume(static_cast<float>(sfx_volume));

	if (!sound_buffers[1].loadFromFile("music/menu-validate.wav"))
	{
		std::cout << "Error loading menu validate sound\n";
		exit(EXIT_FAILURE);
	}

	sounds[1].setBuffer(sound_buffers[1]);
	sounds[1].setVolume(static_cast<float>(sfx_volume));

	if (!sound_buffers[2].loadFromFile("music/menu-back.wav"))
	{
		std::cout << "Error loading menu back sound\n";
		exit(EXIT_FAILURE);
	}

	sounds[2].setBuffer(sound_buffers[2]);
	sounds[2].setVolume(static_cast<float>(sfx_volume));
}