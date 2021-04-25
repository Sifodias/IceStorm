#pragma once
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

class music_struct
{
public:
	music_struct(Mix_Music *m, std::string n) : music(m), name(n) {}

	Mix_Music *music;
	std::string name;
};

namespace Audio_Manager
{
	void init();
	void play(std::string music);

	extern std::vector<music_struct> musics;
};