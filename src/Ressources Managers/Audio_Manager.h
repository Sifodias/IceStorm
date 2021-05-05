#pragma once
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

class music_struct {
public:
	music_struct(Mix_Music* m, std::string n) : music(m), name(n) {}
	Mix_Music* music;
	std::string name;

};

class sound_struct {
public:
	sound_struct(Mix_Chunk* c, std::string n) : chunk(c), name(n) {}
	Mix_Chunk* chunk;
	std::string name;
};

namespace Audio_Manager {
	void init();
	void play(Mix_Music* mus);
	void play(Mix_Chunk* mus, int loops);
	void play(std::string music);
	void quit();
	extern std::vector<music_struct> musics;
	extern std::vector<sound_struct> sounds;
};