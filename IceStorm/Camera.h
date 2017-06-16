#pragma once
#include <SDL.h>

class Camera {
public:
	static void Init();
	static SDL_Rect outerRect;
	static SDL_Rect innerRect;

	static int getX();

	static int getY();

	static int FREEDOM;

};