#pragma once
#include "C_Rect.h"
#include <vector>
#include <SDL.h>
#define CSPEED 150
#define CHAR_H 30
#define CHAR_W 30
class Character
{
public:
	static void handleMoves();
	static void addMoves(SDL_Event& e);
	static void doMoves();
	static void move(SDL_Event& e);
	static void initialize();

	static std::vector<int> movesX;
	static std::vector<int> movesY;	
	static int mainDirection;	//1 = down	| -1 = up
								//2 = right | -2 = left
	static double speedX;
	static double speedY;
	static C_Rect hitBox;
	static SDL_Texture* texture;

	static Uint32 timerA;
	static Uint32 timerB;
};

