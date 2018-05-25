#include "Moving_Unit.h"
#include "Map.h"
#include <iostream>

bool rectEquals(SDL_Rect a, SDL_Rect b) {
	if (a.x == b.x
		&& a.y == b.y
		&& a.w == b.w
		&& a.h == b.h
		)
		return true;
	return false;
}

void speedRestrainer(double& speedX, double& speedY, SDL_Rect rect) {
	rect.x += 1;
	if (speedX > 0 && Map::isItSolid(rect))
		speedX = 0;
	rect.x -= 2;
	if (speedX < 0 && Map::isItSolid(rect))
		speedX = 0;
	rect.x += 1;
	rect.y += 1;
	if (speedY > 0 && Map::isItSolid(rect))
		speedY = 0;
	rect.y -= 2;
	if (speedY < 0 && Map::isItSolid(rect))
		speedY = 0;
}

void Moving_Unit::handleMoves()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	int out = 0;
	while (!out && movesY.size()) {
		switch (movesY.back()) {
		case -1:
			if (state[SDL_SCANCODE_W] && !jumpLock) {
				if (xGRAVITY_ENABLED) {
					speedY = -JSPEED;
					jumpLock = 1;
				}
				else speedY = -CSPEED;
				out = 1;
			}
			else movesY.pop_back();
			break;

		case 1:
			if (state[SDL_SCANCODE_S]) {
				speedY = CSPEED;
				out = 1;
			}
			else movesY.pop_back();
			break;
		}
	}

	out = 0;
	while (!out && movesX.size()) {
		switch (movesX.back()) {
		case 2:
			if (state[SDL_SCANCODE_D]) {
				speedX = CSPEED;
				out = 1;
			}
			else movesX.pop_back();
			break;
		case -2:
			if (state[SDL_SCANCODE_A]) {
				speedX = -CSPEED;
				out = 1;
			}
			else movesX.pop_back();
			break;
		}
	}

	out = 0;
	while (!out && direction.size()) {
		switch (direction.back()) {
		case -1:
			if (state[SDL_SCANCODE_W]) {
				mainDirection = -1;
				out = 1;
			}
			else direction.pop_back();
			break;

		case 1:
			if (state[SDL_SCANCODE_S]) {
				mainDirection = 1;
				out = 1;
			}
			else direction.pop_back();
			break;

		case 2:
			if (state[SDL_SCANCODE_D]) {
				mainDirection = 2;
				out = 1;
			}
			else direction.pop_back();
			break;
		case -2:
			if (state[SDL_SCANCODE_A]) {
				mainDirection = -2;
				out = 1;
			}
			else direction.pop_back();
			break;
		}
	}
	if (!movesX.size()) {
		speedX = 0;
	}

	speedRestrainer(speedX, speedY, hitBox);

	if (!xGRAVITY_ENABLED) {
		jumpLock = 0;
		if (!movesY.size()) {
			speedY = 0;
		}
		return;
	}
	//else if gravity is enabled
	SDL_Rect temprect = hitBox;
	temprect.y += 1;
	if (Map::isItSolid(temprect)) {
		jumpLock = 0;

	}
}

void Moving_Unit::addMoves(SDL_Event & e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			if (!movesY.size())
				movesY.push_back(-1);
			else if (movesY.back() != -1)
				movesY.push_back(-1);
			if (!direction.size())
				direction.push_back(-1);
			else if (direction.back() != -1)
				direction.push_back(-1);
			break;
		case SDLK_s:
			if (!movesY.size())
				movesY.push_back(1);
			else if (movesY.back() != 1)
				movesY.push_back(1);
			if (!direction.size())
				direction.push_back(1);
			else if (direction.back() != 1)
				direction.push_back(1);
			break;
		case SDLK_d:
			if (!movesX.size())
				movesX.push_back(2);
			else if (movesX.back() != 2)
				movesX.push_back(2);
			if (!direction.size())
				direction.push_back(2);
			else if (direction.back() != 2)
				direction.push_back(2);
			break;
		case SDLK_a:
			if (!movesX.size())
				movesX.push_back(-2);
			else if (movesX.back() != -2)
				movesX.push_back(-2);
			if (!direction.size())
				direction.push_back(-2);
			else if (direction.back() != -2)
				direction.push_back(-2);
			break;
		}
	}
}

void Moving_Unit::doMoves()
{
	SDL_Rect tempReqt(hitBox);
	SDL_Rect backup(hitBox);
	timerB = SDL_GetTicks();
	double t = (double)(timerB - timerA)*0.001;
	if (t > 0.100) {
		t = 0;
		timerA = timerB = SDL_GetTicks();
		return;
	}
	if (noclip) {
		hitBox.x += (int)(t*speedX);
		hitBox.y += (int)(t*speedY);
		timerA = timerB;
		return;
	}
	if (speedY <= 300 && xGRAVITY_ENABLED)
		speedY += (int)(GRAVITY*t);


	int tempDistancex = (int)(t*speedX);
	int tempDistancey = (int)(t*speedY);

	//factor technique to get rid of stutter when speedx and y different and hit a wall
	//useless when both speeds are equal
	//glitch: freeze at a distance of 1 pixel for few seconds
	double factorX = abs((double)speedX / (double)(speedX + speedY));
	double factorY = abs((double)speedY / (double)(speedY + speedX));
	factorX = factorY = 1;
	double storageX = 0;
	double storageY = 0;
	while (tempDistancex*speedX > 0 || tempDistancey * speedY > 0) {
		if ((tempDistancex < 0 && speedX > 0) || (tempDistancex > 0 && speedX < 0)) {
			tempDistancex = 0;
			//speedX = 0;
		}
		if ((tempDistancey < 0 && speedY > 0) || (tempDistancey > 0 && speedY < 0)) {
			tempDistancey = 0;
			//speedY = 0;
		}
		tempReqt.x = hitBox.x + tempDistancex;
		tempReqt.y = hitBox.y + tempDistancey;
		if (!Map::isItSolid(tempReqt)) {
			hitBox.x += tempDistancex;
			hitBox.y += tempDistancey;
			break;
		}
		else {
			if (speedX) {
					storageX += (-1)*(speedX / abs(speedX))*factorX;

				if (abs(storageX) >= 1) {
					tempDistancex += (int)((storageX / abs(storageX)));
					storageX -= (storageX / abs(storageX));
				}

			}
			if (speedY) {
					storageY += (-1)*(speedY / abs(speedY))*factorY;

				if (abs(storageY) >= 1) {
					tempDistancey += (int)((storageY / abs(storageY)));
					storageY -= (storageY / abs(storageY));
				}

			}
		}
	}
	
	timerA = timerB;


}

void Moving_Unit::move(SDL_Event & e)
{
	if (movementsLock == 0) {
		addMoves(e);
		handleMoves();
		doMoves();
	}
	if (movementsLock == 1) {
		movementsLock = 0;
		timerA = timerB = SDL_GetTicks();
	}
}

void Moving_Unit::lockMovements()
{
	movementsLock = 1;
}

void Moving_Unit::unlockMovements()
{
	movementsLock = 0;
}

void Moving_Unit::teleport(int x, int y)
{
	hitBox.x = x;
	hitBox.y = y;

}

