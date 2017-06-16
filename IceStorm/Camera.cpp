#include "Camera.h"
#include "Character.h"
#include "Renderer.h"
#include <iostream>

SDL_Rect Camera::innerRect;
SDL_Rect Camera::outerRect;
int Camera::FREEDOM = 1;

void Camera::Init() {
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

int Camera::getX() {
	if (!FREEDOM) {
		if ((int)(Character::movingUnit.hitBox.x + Character::movingUnit.hitBox.w)
		> innerRect.x + innerRect.w)
			innerRect.x = (int)(Character::movingUnit.hitBox.x +
				Character::movingUnit.hitBox.w) - innerRect.w;
		else if ((int)(Character::movingUnit.hitBox.x) < innerRect.x)
			innerRect.x = (int)(Character::movingUnit.hitBox.x);
		outerRect.x = innerRect.x - SCREEN_W / 3;
	}
	else {
		outerRect.x = Character::movingUnit.hitBox.x - (outerRect.w / 2);
	}
	return outerRect.x;
}
int Camera::getY() {
	if (!FREEDOM) {
		if ((int)(Character::movingUnit.hitBox.y + Character::movingUnit.hitBox.h)
		> innerRect.y + innerRect.h)
			innerRect.y = (int)(Character::movingUnit.hitBox.y + Character::movingUnit.hitBox.h)
			- innerRect.h;
		else if ((int)(Character::movingUnit.hitBox.y) < innerRect.y)
			innerRect.y = (int)(Character::movingUnit.hitBox.y);
		outerRect.y = innerRect.y - SCREEN_H / 3;
	}
	else {
		outerRect.y = Character::movingUnit.hitBox.y - (outerRect.h / 2);
	}
	return outerRect.y;
}