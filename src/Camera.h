#pragma once
#include <SDL2/SDL.h>

namespace Camera {
	void init();	
	int getX();
	int getY();
	void lockCamX(int x, bool lock);

	extern SDL_Rect outerRect;
	extern SDL_Rect innerRect;
	extern bool FREEDOM;
};