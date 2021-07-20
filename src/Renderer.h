#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_opengl.h>
// #include <glew

enum RenderMode {
	SDL,
	OPENGL,
	VULKAN
};

namespace Renderer {
	void initAll(RenderMode mode = SDL);		//Inits SDL modules
	void initSDL();
	void initOpenGL();
	void quitAll();		//Closes SDL modules

	extern int SCREEN_W;
	extern int SCREEN_H;
	extern RenderMode mode;
	
	extern SDL_Renderer* g_Renderer;	//Renderer of the engine
	extern SDL_Window* g_Window;		//Window of the game
	extern SDL_GLContext g_context;
};