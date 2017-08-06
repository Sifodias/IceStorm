#pragma once
#include <SDL.h>
#include "C_Rect.h"
#include <vector>
#include "Global_Flags.h"

class Moving_Unit {
public:
	void init(C_Rect hitboxx, int cspeed = CSPEED, int jspeed = JSPEED,
		int gravity = GRAVITY, int gravityEnabled = GRAVITY_ENABLED, int nouclip = 0) {
		hitBox = hitboxx; xCSPEED = cspeed;
		xJSPEED = jspeed; xGRAVITY = gravity; xGRAVITY_ENABLED = gravityEnabled;
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
	int xJSPEED;
	int xCSPEED;
	int xGRAVITY;

	Uint32 timerA;
	Uint32 timerB;
	bool jumpLock;
	bool noclip;
	bool xGRAVITY_ENABLED;
	bool movementsLock;
};
