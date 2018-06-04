#include "Camera.h"
#include "Character.h"
#include "Renderer.h"
#include <iostream>
#include "Map.h"
#include "Objects_Manager.h"

SDL_Rect Camera::innerRect;
SDL_Rect Camera::outerRect;
int Camera::FREEDOM = 0;

void Camera::Init() {
	outerRect.x = outerRect.y = 0;
	outerRect.h = Renderer::SCREEN_H;
	outerRect.w = Renderer::SCREEN_W;
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

		if (outerRect.x < 0)
			return 0;

		double max = 0;
		for (int y = 0; y < Map::matrix[0].size(); y++)
			max = std::fmax(max, Map::matrix[0][y].size());

		double max2 = Map::matrix[0][(Character::movingUnit.hitBox.y / GRID_H)*GRID_H].size()*GRID_W;
		int flagChanged = 0;

		for (int i = 0; i < outerRect.w / 2 + Character::movingUnit.hitBox.x + GRID_W * 90; i += GRID_W) {
			if (!Objects_Manager::findObjectOfID(Map::getIdObject(Character::movingUnit.hitBox.y, 0, i, 0))
				->type.compare("CAMBLOCK")) {
				if (std::abs(Character::movingUnit.hitBox.x - max2) > std::abs(Character::movingUnit.hitBox.x - i)) {
					max2 = i;
					flagChanged = 6666;
				}
			}
		}

		if (!flagChanged) 
			return (int)std::fmin(std::abs(max*GRID_W - outerRect.w), outerRect.x);
		
		if (max2 < Character::movingUnit.hitBox.x)
			return (int)std::fmax(outerRect.x, max2 + GRID_W);

		return (int)std::fmin(outerRect.x, max2 - outerRect.w);
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


		double max2 = std::abs((double)(Map::matrix[0].size()*GRID_H - outerRect.h));
		for (int i = ((outerRect.y / GRID_H))*GRID_H; i < outerRect.h + outerRect.y; i += GRID_H) {
			if (!Objects_Manager::findObjectOfID(Map::getIdObject(i, 0, Character::movingUnit.hitBox.x, 0))
				->type.compare("CAMBLOCK")) {
				max2 = i;
				if (max2 < outerRect.y + outerRect.h / 2)
					goto ret1;
				else
					goto ret2;
				break;
			}
		}
		return (int)std::fmin(std::abs((double)(Map::matrix[0].size()*GRID_H - outerRect.h)), outerRect.y);
	ret1:
		return (int)std::fmax(outerRect.y, max2 + GRID_H);
	ret2:
		return (int)std::fmin(outerRect.y, max2 - outerRect.h);
	}
	else {
		outerRect.y = (int)Character::movingUnit.hitBox.y - (int)(outerRect.h / 2);
		return (int)outerRect.y;
	}
}