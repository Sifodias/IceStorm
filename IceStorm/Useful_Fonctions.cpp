#include "Useful_Fonctions.h"
#include "Renderer.h"
#include "Map.h"
#include "Textures_Manager.h"

void initialize_game()
{
	Renderer::initAll();
	Map::loadLevel();
	Textures_Manager::TMInit();
}

void main_event_loop()
{
	SDL_Event e;
	int out = 0;
	while (out == 0)
	{
		if (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				out = 1;
				Renderer::quitAll();
			}
		}

		SDL_RenderClear(Renderer::g_Renderer);
		Textures_Manager::blitStuff();
		SDL_RenderPresent(Renderer::g_Renderer);

	}
}

std::ifstream loadFromTxt(std::string path)
{
	std::ifstream level_stream;
	level_stream.open(path.c_str());
	if (!level_stream)
	{
		std::cout << "Can't load the stream\n";
	}
	return level_stream;
}