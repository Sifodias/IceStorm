#include "Audio_Manager.h"
#include <filesystem>
#include "Paths.h"
#include <iostream>

std::vector<music_struct> Audio_Manager::musics;

void Audio_Manager::init()
{	
	for (const auto &entry : std::filesystem::directory_iterator(Paths::musicPath))
	{
		if (!std::filesystem::is_regular_file(entry) || entry.path().extension() != ".mp3")
			continue;

		Mix_Music *new_music = Mix_LoadMUS(entry.path().c_str());
		if (new_music == NULL)
		{
			std::cout << "Music not loaded ! Error: " << SDL_GetError() << std::endl;
			continue;
		}

		musics.push_back(music_struct(new_music, entry.path().filename()));
	}
}

void Audio_Manager::play(std::string music){
	for (auto sound: musics){
		if (sound.name == music){
			Mix_PlayMusic(sound.music, -1);
			return;
		}
	}
}