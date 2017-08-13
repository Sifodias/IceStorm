#include "Engine_Manager.h"
#include "Renderer.h"
#include "Map.h"
#include "Textures_Manager.h"
#include "Character.h"
#include <iostream>
#include "Text_Printer.h"
#include "Objects_Manager.h"
#include "Camera.h"
#include "Builder.h"

void Init_game()
{
	Renderer::initAll();
	Map::loadLevel();
	Textures_Manager::Init();
	Objects_Manager::Init();
	Character::Init();
	Text_Printer::Init();
	Camera::Init();
}

void printGrid() {
	for (int y = 0; y < Map::matrix.size()*GRID_H; y += GRID_H) {
		for (int x = 0; x < Map::matrix[0].size()*GRID_W; x += GRID_W) {
			SDL_SetRenderDrawColor(Renderer::g_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLine(Renderer::g_Renderer, x, y, x + (GRID_W*(int)Map::matrix[0].size()), y);
			SDL_RenderDrawLine(Renderer::g_Renderer, x, y, x, y + (GRID_H*(int)Map::matrix.size()));
		}
	}
}

void handleRoutines(SDL_Event e) {
	SDL_RenderClear(Renderer::g_Renderer);
	SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	Builder::routine(e);
	Character::characterRoutine(e);
	Textures_Manager::blitStuff();

	Text_Printer::handleRoutine(e);

	SDL_RenderPresent(Renderer::g_Renderer);
}

void main_event_loop()
{
	SDL_Event e;
	int out = 0;
	while (out == 0) {
		if (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				out = 1;
				Renderer::quitAll();
				break;
			}
		}
		handleRoutines(e);
	}
}

std::ifstream loadFile(std::string path)
{
	std::ifstream level_stream;
	level_stream.open(path.c_str());
	if (!level_stream) {
		std::cout << "Can't load the stream at path : " << path << std::endl;
	}
	return level_stream;
}

void engineQuit() {
	if (SAVE_ENABLED)
		Map::saveMatrix();

}