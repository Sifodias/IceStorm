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
	for (int y = 0; y < Map::y*GRID_H; y += GRID_H) {
		for (int x = 0; x < Map::x*GRID_W; x += GRID_W) {
			SDL_SetRenderDrawColor(Renderer::g_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLine(Renderer::g_Renderer, x, y, x + (GRID_W*Map::x), y);
			SDL_RenderDrawLine(Renderer::g_Renderer, x, y, x, y + (GRID_H*Map::y));
		}
	}
}

void handleRoutines(SDL_Event e) {
	SDL_RenderClear(Renderer::g_Renderer);
	SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	Character::characterRoutine(e);
	Builder::routine(e);
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
	Map::saveMatrix();

}