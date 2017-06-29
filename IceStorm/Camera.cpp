#include "Camera.h"
#include "Character.h"
#include "Renderer.h"
#include <iostream>

SDL_Rect Camera::innerRect;
SDL_Rect Camera::outerRect;
int Camera::FREEDOM = 0;

void Camera::Init() {
	outerRect.x = outerRect.y = 0;
	outerRect.h = Renderer::SCREEN_H; outerRect.w = Renderer::SCREEN_W;
	innerRect.x = outerRect.x + Renderer::SCREEN_W / 3;
	innerRect.y = outerRect.y + Renderer::SCREEN_H / 3;
	innerRect.h = Renderer::SCREEN_H / 3; innerRect.w = Renderer::SCREEN_W / 3;
}

int Camera::getX() {
	if (!FREEDOM) {
		if ((int)(Character::movingUnit.hitBox.x + Character::movingUnit.hitBox.w)
		> innerRect.x + innerRect.w)
			innerRect.x = (int)(Character::movingUnit.hitBox.x +
				Character::movingUnit.hitBox.w) - innerRect.w;
		else if ((int)(Character::movingUnit.hitBox.x) < innerRect.x)
			innerRect.x = (int)(Character::movingUnit.hitBox.x);
		outerRect.x = innerRect.x - Renderer::SCREEN_W / 3;
	}
	else {
		outerRect.x = Character::movingUnit.hitBox.x - (outerRect.w / 2);
	}
	if (outerRect.x < 0)
		return 0;
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
		outerRect.y = innerRect.y - Renderer::SCREEN_H / 3;
	}
	else {
		outerRect.y = Character::movingUnit.hitBox.y - (outerRect.h / 2);
	}
	if (outerRect.y < 0)
		return 0;
	return outerRect.y;
}