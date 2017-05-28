#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <ostream>
#include "Engine_Manager.h"
#include <iostream>
#include "Textures_Manager.h"
#include "Renderer.h"
#include "Paths.h"
#include <array>

#define PRINT_SPEED 0

typedef struct t_nodequeue {
	std::string str;
	SDL_Rect rect;
	int policeID;
	int iterator;
	SDL_Rect container;
	bool lock;
} NodeQueue;

class Text_Printer {
public:
	static void Init();
	static void addToQueue(std::string str, 
		SDL_Rect* container = NULL, int policeID = 0, SDL_Rect* rect = NULL);
	static void handleRoutine(SDL_Event e);

	static std::vector<std::array<SDL_Texture*, 127>> lettersVec;
	static std::vector<NodeQueue> queue;

	static bool busy;
private:
	static void printText(NodeQueue& node);
	static Uint32 timerA;
	static Uint32 timerB;
	static SDL_Rect defaultRect;
	static SDL_Rect defaultContainer;
	static int flagOverflow;
	static SDL_Texture* dialogBox;
	static SDL_Rect dialogRect;
};

