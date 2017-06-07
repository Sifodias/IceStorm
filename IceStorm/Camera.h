#pragma once
#include <SDL.h>
#include "Renderer.h"
#include "Character.h"

class Camera {
public:
	static void Init() {
		outerRect.x = outerRect.y = 0;
		outerRect.h = SCREEN_H; outerRect.w = SCREEN_W;
		innerRect.x = outerRect.x + SCREEN_W / 3;
		innerRect.y = outerRect.y + SCREEN_H / 2;
		innerRect.h = SCREEN_W / 2; innerRect.w = SCREEN_W / 3;
	}
	static SDL_Rect outerRect;
	static SDL_Rect innerRect;

	static int getX() {
		if ((int)(Character::hitBox.x + Character::hitBox.w) > innerRect.x + innerRect.w)
			innerRect.x = (int)(Character::hitBox.x + Character::hitBox.w) - innerRect.w;
		else if ((int)(Character::hitBox.x) < innerRect.x)
			innerRect.x = (int)(Character::hitBox.x);
		outerRect.x = innerRect.x - SCREEN_W / 3;
		return outerRect.x;
	}
	static int getY() {
		if ((int)(Character::hitBox.y + Character::hitBox.h) > innerRect.y + innerRect.h)
			innerRect.y = (int)(Character::hitBox.y + Character::hitBox.h) - innerRect.h;
		else if ((int)(Character::hitBox.y) < innerRect.y)
			innerRect.y = (int)(Character::hitBox.y);

		outerRect.y = innerRect.y - SCREEN_H / 2;
		return outerRect.y;
	}
};