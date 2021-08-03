#include "Camera.h"
#include "Character.h"
#include "Renderer.h"
#include <iostream>
 
#include "Objects_m.h"

SDL_Rect Camera::innerRect;
SDL_Rect Camera::outerRect;
bool Camera::FREEDOM = 1;
int locked = false;
int lockX = 0;

void Camera::init() {
	outerRect.x = outerRect.y = 0;
	outerRect.h = Renderer::SCREEN_H;
	outerRect.w = Renderer::SCREEN_W;
}

std::tuple<int, int> Camera::getCoord() {
	return { getX(), getY() };
}

int Camera::getX() {	
	outerRect.x = (int)Character::movingUnit.hitBox.x - (int)(outerRect.w / 2);
	return (int)outerRect.x;
}

int Camera::getY() {
	outerRect.y = (int)Character::movingUnit.hitBox.y - (int)(outerRect.h / 2);
	return (int)outerRect.y;
}

void Camera::lockCamX(int x, bool lock) {
	locked = lock;
	lockX = x;
}