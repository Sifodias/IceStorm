#include "Camera.h"
#include "Character.h"
#include "Renderer.h"
#include <iostream>
#include "Map.h"
#include "Objects_Manager.h"

SDL_Rect Camera::innerRect;
SDL_Rect Camera::outerRect;
bool Camera::FREEDOM = 0;
int locked = false;
int lockX = 0;

void Camera::init() {
	outerRect.x = outerRect.y = 0;
	outerRect.h = Renderer::SCREEN_H;
	outerRect.w = Renderer::SCREEN_W;
	innerRect.x = outerRect.x + Renderer::SCREEN_W / 3;
	innerRect.y = outerRect.y + Renderer::SCREEN_H / 3;
	innerRect.h = Renderer::SCREEN_H / 3; innerRect.w = Renderer::SCREEN_W / 3;
}

//the get functions need some reworking to make the camblock feature work
//this feature is currently disabled
int Camera::getX() {
	if (!FREEDOM) {
		if (locked) {
			return (outerRect.w / 2) - lockX;
		}


		if ((int)(Character::movingUnit.hitBox.x + Character::movingUnit.hitBox.w)
		> innerRect.x + innerRect.w)
			innerRect.x = (int)(Character::movingUnit.hitBox.x +
				Character::movingUnit.hitBox.w) - innerRect.w;

		else if ((int)(Character::movingUnit.hitBox.x) < innerRect.x)
			innerRect.x = (int)(Character::movingUnit.hitBox.x);

		outerRect.x = innerRect.x - Renderer::SCREEN_W / 3;

		if (outerRect.x < 0)
			return 0;

		double max1 = 0;
		for (int y = 0; y < Map::matrix[0].size(); y++)
			max1 = std::max(max1, (double)Map::matrix[0][y].size());


		double max2 = outerRect.w / 2 + Character::movingUnit.hitBox.x + GRID_W * 5;
		int flagChanged = 0;
		int r = 0, l = 0;
		goto bypass;
		for (int i = (Character::movingUnit.hitBox.x / GRID_W)*GRID_W - outerRect.w / 2 - GRID_W * 5;
			i < outerRect.w / 2 + Character::movingUnit.hitBox.x + GRID_W * 5; i += GRID_W) {
			if (!Objects_Manager::findObject(Map::getIdObject(Character::movingUnit.hitBox.y, 0, i, 0))
				.type.compare("CAMBLOCKX")) {
				if (std::abs(Character::movingUnit.hitBox.x - max2) > std::abs(Character::movingUnit.hitBox.x - i)) {
					max2 = i;
					flagChanged = 6666;
				}
				if (i < Character::movingUnit.hitBox.x) {
					if (abs(r - Character::movingUnit.hitBox.x) > abs(i - Character::movingUnit.hitBox.x))
						r = i;
				}
				else
					if (abs(l - Character::movingUnit.hitBox.x) > abs(i - Character::movingUnit.hitBox.x))
						l = i;
			}
		}
		if (r && l)
			return ((r + l) / 2) - (outerRect.w) / 2 + GRID_W / 2;
	bypass:
		if (!flagChanged)
			return (int)std::min(std::abs(max1*GRID_W - outerRect.w), (double)outerRect.x);

		if (max2 < Character::movingUnit.hitBox.x)
			return (int)std::max((double)outerRect.x, max2 + GRID_W);

		return (int)std::min((double)outerRect.x, max2 - outerRect.w);
	}

	else {
		outerRect.x = (int)Character::movingUnit.hitBox.x - (int)(outerRect.w / 2);
		return (int)outerRect.x;
	}
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

		if (outerRect.y < 0)
			return 0;


		double max2 = std::abs((double)(Map::matrix[0].size()*GRID_H));
		int flagChanged = 0;
		int r = 0, l = 0;
		goto bypass;
		for (int i = (Character::movingUnit.hitBox.y / GRID_H)*GRID_H - outerRect.h / 2 - GRID_H * 5;
			i < outerRect.h / 2 + Character::movingUnit.hitBox.y + GRID_H * 5; i += GRID_H) {
			if (!Objects_Manager::findObject(Map::getIdObject(i, 0, Character::movingUnit.hitBox.x, 0))
				.type.compare("CAMBLOCKY")) {
				if (std::abs(Character::movingUnit.hitBox.y - max2) > std::abs(Character::movingUnit.hitBox.y - i)) {
					max2 = i;
					flagChanged = 6666;
				}
				if (i < Character::movingUnit.hitBox.y) {
					if (abs(r - Character::movingUnit.hitBox.y) > abs(i - Character::movingUnit.hitBox.y))
						r = i;
				}
				else
					if (abs(l - Character::movingUnit.hitBox.y) > abs(i - Character::movingUnit.hitBox.y))
						l = i;
			}
		}
		if (r && l)
			return ((r + l) / 2) - (outerRect.h) / 2 + GRID_H / 2;
	bypass:
		if (!flagChanged)
			return (int)std::min(std::abs((double)(Map::matrix[0].size()*GRID_H - outerRect.h)), (double)outerRect.y);

		if (max2 < Character::movingUnit.hitBox.y)
			return (int)std::max((double)outerRect.y, max2 + GRID_H);

		return (int)std::min((double)outerRect.y, max2 - outerRect.h);
	}

	else {
		outerRect.y = (int)Character::movingUnit.hitBox.y - (int)(outerRect.h / 2);
		return (int)outerRect.y;
	}
}

void Camera::lockCamX(int x, bool lock) {
	locked = lock;
	lockX = x;
}