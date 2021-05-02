#include "Audio_Manager.h"
#include <filesystem>
#include "Paths.h"
#include <iostream>

std::vector<music_struct> Audio_Manager::musics;
std::vector<sound_struct> Audio_Manager::sounds;

void Audio_Manager::init()
{
	for (const auto &entry : std::filesystem::directory_iterator(Paths::musicPath))
	{
		if (!std::filesystem::is_regular_file(entry))
			continue;

		if (entry.path().extension() == ".mp3")
		{
			Mix_Music *new_music = Mix_LoadMUS(entry.path().c_str());
			if (new_music == NULL)
			{
				std::cout << "Music not loaded ! Error: " << SDL_GetError() << std::endl;
				continue;
			}

			musics.push_back(music_struct(new_music, entry.path().filename()));
		}
		if (entry.path().extension() == ".wav")
		{

			Mix_Chunk *new_chunk = Mix_LoadWAV(entry.path().c_str());
			if (new_chunk == NULL)
			{
				std::cout << "Sound not loaded ! Error: " << SDL_GetError() << std::endl;
				continue;
			}

			sounds.push_back(sound_struct(new_chunk, entry.path().filename()));
		}
	}
}

void Audio_Manager::play(Mix_Music *mus)
{
	Mix_PlayMusic(mus, -1);
}

void Audio_Manager::play(Mix_Chunk *chunk, int loops=0)
{
	Mix_PlayChannel(-1, chunk, loops);
}

void Audio_Manager::play(std::string music)
{
	for (auto sound : musics)
	{
		if (sound.name == music)
		{
			play(sound.music);
			return;
		}
	}
	for (auto sound : sounds)
	{
		if (sound.name == music)
		{
			play(sound.chunk);
			return;
		}
	}
	std::cout << "No music/sound found with name: " << music << std::endl;
}