#include "Renderer.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#define FULLSCREEN 1

using namespace std;

SDL_Renderer* Renderer::g_Renderer = NULL;
SDL_Window* Renderer::g_Window = NULL;
SDL_GLContext Renderer::g_context;

int Renderer::SCREEN_W = 320;
int Renderer::SCREEN_H = 240;
RenderMode Renderer::mode = SDL;

void Renderer::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0)
		std::cout << "Can't load the SDL: " << SDL_GetError() << endl;

	if (!IMG_Init(IMG_INIT_PNG))
		std::cout << "Can't load SDL_Image: " << SDL_GetError() << endl;

	if (TTF_Init() == -1)
		std::cout << "Can't load SDL_ttf: " << TTF_GetError() << endl;

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
			SDL_WINDOWPOS_UNDEFINED,
			//((int)(current.h / SCREEN_H) ) * SCREEN_W,
			current.w,
			//((int)(current.h / SCREEN_H)) * SCREEN_H,
			current.h - 150,
			flags);
	}

	if (g_Window == NULL)
		std::cout << "Can't create the window: " << SDL_GetError() << endl;

	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_Renderer == NULL)
		std::cout << "Can't create the renderer: " << SDL_GetError() << endl;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(g_Renderer, SCREEN_W, SCREEN_H);
	SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void Renderer::initOpenGL(){
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* g_Window = SDL_CreateWindow("OpenGL Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_OPENGL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	g_context = SDL_GL_CreateContext(g_Window);
}

void Renderer::initAll(RenderMode m) {
	mode = m;
	switch (mode) {
	case SDL:
	initSDL();
	break;

	case OPENGL:
	initOpenGL();
	break;

	}
}


void Renderer::quitAll() {
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

