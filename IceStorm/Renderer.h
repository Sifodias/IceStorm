#pragma once
#include <SDL.h>

class Renderer {
public:
	//Renderer();

	static void initAll();		//Initializes SDL modules
	static void quitAll();		//Closes SDL modules
	
	static SDL_Renderer* g_Renderer;	//Renderer of the engine
	static SDL_Window* g_Window;		//Window of the game

};