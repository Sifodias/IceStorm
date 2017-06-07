#pragma once
#include <SDL.h>
#include "Renderer.h"
#include "Character.h"
#include <iostream>
#define FREEDOM 1
class Camera {
public:
	static void Init() {
		outerRect.x = outerRect.y = 0;
		outerRect.h = SCREEN_H; outerRect.w = SCREEN_W;
		if (FREEDOM) {
			innerRect.x = outerRect.x + SCREEN_W / 3;
			innerRect.y = outerRect.y + SCREEN_H / 3;
			innerRect.h = SCREEN_H / 3; innerRect.w = SCREEN_W / 3;
		}
		else {
			innerRect.x = outerRect.x + SCREEN_W / 3;
			innerRect.y = outerRect.y + SCREEN_H / 3;
			innerRect.h = 0; innerRect.w = 0;
		}
	}
	static SDL_Rect outerRect;
	static SDL_Rect innerRect;

	static int getX() {
		if ((int)(Character::hitBox.x + Character::hitBox.w) > innerRect.x + innerRect.w)
			innerRect.x = (int)(Character::hitBox.x + Character::hitBox.w) - innerRect.w;
		else if ((int)(Character::hitBox.x) < innerRect.x)
			innerRect.x = (int)(Character::hitBox.x);
	//	if (FREEDOM)
			outerRect.x = innerRect.x - SCREEN_W / 3;
		//else outerRect.x = innerRect.x;
		return outerRect.x;
	}
	static int getY() {
		if ((int)(Character::hitBox.y + Character::hitBox.h) > innerRect.y + innerRect.h)
			innerRect.y = (int)(Character::hitBox.y + Character::hitBox.h) - innerRect.h;
		else if ((int)(Character::hitBox.y) < innerRect.y)
			innerRect.y = (int)(Character::hitBox.y);
		//if (FREEDOM)
			outerRect.y = innerRect.y - SCREEN_H / 3;
		//else outerRect.y = innerRect.y;
		return outerRect.y;
	}
};