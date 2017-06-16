#pragma once
#include <SDL.h>
#include "C_Rect.h"
#include <vector>

class Moving_Unit {
public:
	void init(C_Rect hitboxx, int cspeed = 150, int jspeed = 350,
		int gravity = 1300, int gravityEnabled = 1, int nouclip = 0) {
		hitBox = hitboxx; CSPEED = cspeed;
		JSPEED = jspeed; GRAVITY = gravity; GRAVITY_ENABLED = gravityEnabled;
		noclip = nouclip; mainDirection = 2; jumpLock = 0; movementsLock = 1;
	}

	void handleMoves();
	void addMoves(SDL_Event& e);
	void doMoves();
	void move(SDL_Event& e);
	void lockMovements();
	void unlockMovements();

	double speedX;
	double speedY;
	C_Rect hitBox;
	std::vector<int> movesX;
	std::vector<int> movesY;
	int mainDirection;
	std::vector<int> direction;		//1 = down	| -1 = up
									//2 = right | -2 = left
	int JSPEED;
	int CSPEED;
	int GRAVITY;

	Uint32 timerA;
	Uint32 timerB;
	bool jumpLock;
	bool noclip;
	bool GRAVITY_ENABLED;
	bool movementsLock;
};
