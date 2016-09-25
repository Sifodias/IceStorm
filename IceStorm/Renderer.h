#pragma once
#include <SDL.h>
#include <string>
class Renderer {
public:
	//Renderer();

	static void initAll();		//Initialize SDL modules
	static void quitAll();		//Close SDL modules

	void renderObjects();
	
	static SDL_Renderer* g_Renderer;	//Renderer of the engine
	static SDL_Window* g_Window;		//Window of the game

};