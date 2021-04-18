#pragma once
#include <SDL2/SDL.h>
#include <string>

namespace Renderer {
	void initAll();		//Inits SDL modules
	void quitAll();		//Closes SDL modules

	extern int SCREEN_W;
	extern int SCREEN_H;
	
	extern SDL_Renderer* g_Renderer;	//Renderer of the engine
	extern SDL_Window* g_Window;		//Window of the game

};