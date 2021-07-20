#pragma once
#include <SDL2/SDL.h>
#include <tuple>

namespace Camera {
	void init();	
	std::tuple<int, int> getCoord();
	int getX();
	int getY();
	void lockCamX(int x, bool lock);

	extern SDL_Rect outerRect;
	extern SDL_Rect innerRect;
	extern bool FREEDOM;
};