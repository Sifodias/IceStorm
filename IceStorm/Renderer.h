#pragma once
#include <SDL.h>
#include <string>

class Renderer {
public:
	//Renderer();

	static void initAll();		//Inits SDL modules
	static bool saveScreenshotBMP(std::string filepath);
	static void quitAll();		//Closes SDL modules
	static int SCREEN_W;
	static int SCREEN_H;

	static SDL_Renderer* g_Renderer;	//Renderer of the engine
	static SDL_Window* g_Window;		//Window of the game

};