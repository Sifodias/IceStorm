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

#define PRINT_SPEED 100

typedef struct t_nodequeue {
	std::string str;
	SDL_Rect rect;
	int policeID;
	int iterator;
	bool hold;
} NodeQueue;

class Text_Printer {
public:
	static void Init();
	static void printText(NodeQueue& node);


	static void addToQueue(std::string, 
		SDL_Rect* rect = NULL, int policeID = 1, bool hold = 1);
	static void handleRoutine();

	static std::vector<std::array<SDL_Texture*, 127>> lettersVec;
	static std::vector<NodeQueue> queue;


	static Uint32 timerA;
	static Uint32 timerB;
	static SDL_Rect defaultRect;
};

