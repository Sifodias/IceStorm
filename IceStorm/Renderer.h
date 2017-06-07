#pragma once
#include <SDL.h>
#define SCREEN_W 320
#define SCREEN_H 240

class Renderer {
public:
	//Renderer();

	static void initAll();		//Inits SDL modules
	static void quitAll();		//Closes SDL modules
	
	static SDL_Renderer* g_Renderer;	//Renderer of the engine
	static SDL_Window* g_Window;		//Window of the game

};