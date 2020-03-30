#pragma once
#include <SDL.h>

namespace Camera {
	void init();	
	int getX();
	int getY();

	extern SDL_Rect outerRect;
	extern SDL_Rect innerRect;
	extern bool FREEDOM;
};