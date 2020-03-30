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
#include "Events_Manager.h"



void init_game()
{
	Renderer::initAll();
	Map::loadLevel("maintest.txt");
	Textures_Manager::init();
	Objects_Manager::init();
	Text_Printer::init();
	Camera::init();
	Character::init();
	//Events_Manager::addToQueue(testTitle);

	//SDL_Thread* routineThread = SDL_CreateThread(blitting, "titi", (void*)NULL);
	//SDL_DetachThread(routineThread);

}

int filterEvents(SDL_Event* e) {
	if (e->type != SDL_KEYDOWN && SDL_KEYUP && SDL_QUIT && SDL_SYSWMEVENT && SDL_WINDOWEVENT)
		return 1;	//not cool event
	return 0;	//cool event
}


void handleRoutines(SDL_Event e) {
	
	SDL_RenderClear(Renderer::g_Renderer);
	Builder::routine(e);
	Character::characterRoutine(e);
	Events_Manager::routine();
	Textures_Manager::blitStuff();
	Text_Printer::handleRoutine(e);
	SDL_SetRenderDrawColor(Renderer::g_Renderer, 100, 50, 100, SDL_ALPHA_OPAQUE);
	SDL_RenderPresent(Renderer::g_Renderer);


}

int main_event_loop(void*)
{
	SDL_Event e;
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
	return 0;
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
