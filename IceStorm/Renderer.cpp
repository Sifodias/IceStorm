#include "Renderer.h"
#include <iostream>
#include <SDL_image.h>
//#include <SDL_mixer.h>
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
			SDL_WINDOWPOS_UNDEFINED, ((int)(current.h / SCREEN_H)-1) * SCREEN_W,
			((int)(current.h / SCREEN_H)-1) * SCREEN_H, flags);
	}
	if (g_Window == NULL) {
		printf("Can't create the window : %s\n", SDL_GetError());
	}

	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_Renderer == NULL) {
		printf("Can't create the renderer : %s\n", SDL_GetError());
	}

	SDL_RenderSetLogicalSize(g_Renderer, SCREEN_W, SCREEN_H);
}

bool Renderer::saveScreenshotBMP(std::string filepath) {
	SDL_Surface* saveSurface = NULL;
	SDL_Surface* infoSurface = NULL;
	infoSurface = SDL_GetWindowSurface(g_Window);


	if (infoSurface == NULL) {
		std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
	}
	else {
		unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
		if (pixels == 0) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
			return false;
		}
		else {
			if (SDL_RenderReadPixels(g_Renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
				pixels = NULL;
				return false;
			}
			else {
				saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
				if (saveSurface == NULL) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
					return false;
				}
				SDL_SaveBMP(saveSurface, filepath.c_str());
				SDL_FreeSurface(saveSurface);
				saveSurface = NULL;
			}
			delete[] pixels;
		}
		SDL_FreeSurface(infoSurface);
		infoSurface = NULL;
	}
	return true;
}

void Renderer::quitAll()
{
	engineQuit();
	SDL_DestroyWindow(g_Window);
	SDL_DestroyRenderer(g_Renderer);
	IMG_Quit();
	TTF_Quit();
	//Mix_Quit();
	SDL_Quit();
}

