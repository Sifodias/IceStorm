#pragma once
#include "C_Rect.h"
#include <vector>
#include <SDL.h>
#include "Sprites_Handler.h"
#define CSPEED 150

#define JSPEED 350	//jump speed
#define GRAVITY 1300
#define JUMPLOCK 1
#define GRAVITY_ENABLED 1

#define CHAR_H 30
#define CHAR_W 20
#define CHAR_HITBOX_H 30
#define CHAR_HITBOX_W 20

class Character
{
public:
	static void handleMoves();
	static void addMoves(SDL_Event& e);
	static void doMoves();
	static void move(SDL_Event& e);
	static void characterRoutine(SDL_Event& e);
	static void Init();

	static std::vector<int> movesX;
	static std::vector<int> movesY;	
	static int mainDirection;
	static std::vector<int> direction;	//1 = down	| -1 = up
										//2 = right | -2 = left
	static double speedX;
	static double speedY;
	static C_Rect hitBox;
	
	static SpritesHandler textures;

	static bool jumpLock;
	static bool noclip;

	static Uint32 timerA;
	static Uint32 timerB;
};

