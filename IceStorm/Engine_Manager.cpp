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
	Map::loadLevel("default.txt");
	Textures_Manager::Init();
	Objects_Manager::Init();
	Character::Init();
	Text_Printer::Init();
	Camera::Init();
}

/*
void printGrid() {
	SDL_SetRenderDrawColor(Renderer::g_Renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int y = 0; y < Map::matrix.size()*GRID_H; y += GRID_H) {
		for (int x = 0; x < Map::matrix[y / GRID_H].size()*GRID_W; x += GRID_W) {
			SDL_RenderDrawLine(Renderer::g_Renderer, x, y, x + GRID_W*(int)Map::matrix[y / GRID_H].size(), y);
			SDL_RenderDrawLine(Renderer::g_Renderer, x, y, x, y + GRID_H*(int)Map::matrix.size());
		}
	}
}*/

int filterEvents(SDL_Event* e) {
	if (e->type != SDL_KEYDOWN && SDL_KEYUP && SDL_QUIT && SDL_SYSWMEVENT && SDL_WINDOWEVENT)
		return 1;	//not cool event
	return 0;	//cool event
}

void handleRoutines(SDL_Event e) {
	SDL_RenderClear(Renderer::g_Renderer);

	Builder::routine(e);
	Character::characterRoutine(e);

	Textures_Manager::blitStuff();

	Text_Printer::handleRoutine(e);

	SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderPresent(Renderer::g_Renderer);
}

void main_event_loop()
{
	SDL_Event e;

	Uint32 timerA = SDL_GetTicks();
	Uint32 timerB = SDL_GetTicks();
	int x = Character::movingUnit.hitBox.x;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			SDL_FlushEvent(SDL_MOUSEMOTION);			//This useless event overloads the event queue
			if (e.type == SDL_QUIT) {
				Renderer::quitAll();
				break;
			}

		}

		handleRoutines(e);
	}



}

std::ifstream* loadFile(std::string path)
{
	std::ifstream* level_stream = new ifstream;
	level_stream->open(path.c_str());
	if (!(*level_stream)) {
		std::cout << "Can't load the stream at path : " << path << std::endl;
		level_stream = NULL;
	}
	return level_stream;
}

void engineQuit() {
	if (SAVE_ENABLED) {
		Map::saveMatrix();
		Objects_Manager::saveObjects();
	}

}
