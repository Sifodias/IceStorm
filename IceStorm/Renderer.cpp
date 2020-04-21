#include "Renderer.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Engine_Manager.h"
#include <SDL_ttf.h>
#include <string>
#include "Map.h"

#define FULLSCREEN 0

SDL_Renderer* Renderer::g_Renderer = NULL;
SDL_Window* Renderer::g_Window = NULL;
int Renderer::SCREEN_W = 320;
int Renderer::SCREEN_H = 240;


void Renderer::initAll()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		std::cout << "Can't load the SDL: " << SDL_GetError() << endl;
	
	if (!IMG_Init(IMG_INIT_PNG))
		std::cout << "Can't load SDL_Image: "<< SDL_GetError() << endl;
	
	if (TTF_Init() == -1)
		std::cout << "Can't load SDL_ttf: "<< TTF_GetError() << endl;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		std::cout << "Can't load SDL_mixer: " << Mix_GetError() << endl;

	SDL_DisplayMode current;
	if (SDL_GetCurrentDisplayMode(0, &current))
		std::cout << SDL_GetError() << endl;
	
	Uint32 flags;
	if (FULLSCREEN) {
		flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
		g_Window = SDL_CreateWindow("The Rising Shadows Engine", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, current.w, current.h, flags);
	}
	else {
		flags = SDL_WINDOW_SHOWN;
		g_Window = SDL_CreateWindow("The Rising Shadows Engine", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, ((int)(current.h / SCREEN_H)-2) * SCREEN_W,
			((int)(current.h / SCREEN_H)-2) * SCREEN_H, flags);
	}

	if (g_Window == NULL) 
		std::cout << "Can't create the window: " << SDL_GetError() << endl;
	
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_Renderer == NULL)
		std::cout << "Can't create the renderer: "<< SDL_GetError() << endl;
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(g_Renderer, SCREEN_W, SCREEN_H);
	SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}


void Renderer::quitAll()
{
	engineQuit();
	SDL_DestroyWindow(g_Window);
	SDL_DestroyRenderer(g_Renderer);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

