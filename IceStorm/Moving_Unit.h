#pragma once
#include <vector>
#include "Global_Flags.h"
#include <SDL.h>

class Moving_Unit {
public:
	Moving_Unit(SDL_Rect hitbox_i = SDL_Rect{ 0, 0, 0, 0 }, int cspeed = CSPEED, int jspeed = JSPEED,
		int gravityEnabled = GRAVITY_ENABLED, int noclip_i = 0);

	void handleMoves();
	void addMoves(SDL_Event& e);
	void doMoves();
	void move(SDL_Event& e);
	void lockMovements();
	void unlockMovements();
	void teleport(int x, int y);

	double speedX;
	double speedY;
	SDL_Rect hitBox;
	std::vector<int> movesX;
	std::vector<int> movesY;
	int mainDirection;
	std::vector<int> direction;		//1 = down	| -1 = up
									//2 = right | -2 = left
	int jump_speed;
	int move_speed;

	Uint32 timerA;
	Uint32 timerB;
	bool jumpLock;
	bool noclip;
	bool gravity_affected;
	bool movementsLock;
};
