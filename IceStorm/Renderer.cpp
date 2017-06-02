#include "Renderer.h"
#include <iostream>
#include <SDL_image.h>
//#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#define FULLSCREEN 0

SDL_Renderer* Renderer::g_Renderer = NULL;
SDL_Window* Renderer::g_Window = NULL;


void Renderer::initAll()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("Can't load the SDL : %s\n", SDL_GetError());
	}

	if (!IMG_Init(IMG_INIT_PNG)) {
		printf("Can't load SDL_Image : %s\n", SDL_GetError());
	}

	if (TTF_Init() == -1) {
		printf("SDL_ttf could not Init! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	//{
	//	printf("SDL_mixer couldn't Init : %s\n", Mix_GetError());
	//}
	SDL_DisplayMode current;
	if (SDL_GetCurrentDisplayMode(0, &current)) {
		std::cout << SDL_GetError();
	}
	Uint32 flags;
	if (FULLSCREEN) {
		flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
		g_Window = SDL_CreateWindow("The Rising Shadows Engine", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, current.w, current.h, flags);
	}
	else {
		flags = SDL_WINDOW_SHOWN;
		g_Window = SDL_CreateWindow("The Rising Shadows Engine", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 640, 480, flags);
	}
	if (g_Window == NULL) {
		printf("Can't create the window : %s\n", SDL_GetError());
	}

	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_Renderer == NULL) {
		printf("Can't create the renderer : %s\n", SDL_GetError());
	}

	SDL_RenderSetLogicalSize(g_Renderer, 320, 240);
}


void Renderer::quitAll()
{
	SDL_DestroyWindow(g_Window);
	SDL_DestroyRenderer(g_Renderer);
	IMG_Quit();
	TTF_Quit();
	//Mix_Quit();
	SDL_Quit();
}

