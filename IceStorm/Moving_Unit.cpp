#include "Moving_Unit.h"
#include "Map.h"

void Moving_Unit::handleMoves()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	int out = 0;
	while (!out && movesY.size()) {
		switch (movesY.back()) {
		case -1:
			if (state[SDL_SCANCODE_W] && !jumpLock) {
				if (GRAVITY_ENABLED) {
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
	if (!GRAVITY_ENABLED) {
		if (!movesY.size()) {
			speedY = 0;
		}
		return;
	}
	//else if gravity is enabled
	C_Rect temprect = hitBox;
	temprect.y += 1;
	if (Map::isItSolid(temprect) && speedY > 0) {
		speedY = 0;
		if (!state[SDL_SCANCODE_W])
			jumpLock = 0;
		return;
	}
	temprect.y -= 2;
	if (Map::isItSolid(temprect) && speedY < 0) {
		speedY = 0;
		if (!state[SDL_SCANCODE_W])
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
	C_Rect tempReqt(hitBox);
	C_Rect backup(hitBox);
	timerB = SDL_GetTicks();
	double t = (double)(timerB - timerA)*0.001;
	if (noclip) {
		hitBox.x += t*speedX;
		hitBox.y += t*speedY;
		timerA = timerB = SDL_GetTicks();
		return;
	}
	if (speedY <= 300 && GRAVITY_ENABLED)
		speedY += (int)(GRAVITY*t);
	int out = 0;
	if (!speedY) {
		double tempDistance = t*speedX;
		while (!out && tempDistance*speedX >= 0) {
			tempReqt.x = hitBox.x + tempDistance;
			if (!Map::isItSolid(tempReqt)) {
				hitBox.x += tempDistance;
				out = 1;
			}
			else tempDistance += (-1)*(speedX / abs(speedX));
		}
	}
	else if (!speedX) {
		double tempDistance = t*speedY;
		while (!out && tempDistance*speedY >= 0) {
			tempReqt.y = hitBox.y + tempDistance;
			if (!Map::isItSolid(tempReqt)) {
				hitBox.y += tempDistance;
				out = 1;
			}
			else tempDistance += (-1)*(speedY / abs(speedY));
		}
	}
	else {
		double tempDistanceY = t*speedY;
		double tempDistanceX = t*speedX;
		while (!out && tempDistanceX*speedX >= 0 && tempDistanceY*speedY >= 0) {
			tempReqt.x = hitBox.x + tempDistanceX;
			tempReqt.y = hitBox.y + tempDistanceY;
			if (!Map::isItSolid(tempReqt)) {
				hitBox.y += tempDistanceY;
				hitBox.x += tempDistanceX;
				out = 1;
			}
			else {
				tempDistanceX += (-1)*(speedX / abs(speedX));
				tempDistanceY += (-1)*(speedY / abs(speedY));
			}

		}
		if (backup == hitBox) {
			out = 0;
			double tempDistance = t*speedX;
			while (!out && tempDistance*speedX >= 0) {
				tempReqt.x = hitBox.x + tempDistance;
				tempReqt.y = hitBox.y;
				if (!Map::isItSolid(tempReqt)) {
					hitBox.x += tempDistance;
					out = 1;
				}
				else tempDistance += (-1)*(speedX / abs(speedX));
			}

			tempDistance = t*speedY;
			out = 0;
			while (!out && tempDistance*speedY >= 0) {
				tempReqt.y = hitBox.y + tempDistance;
				tempReqt.x = hitBox.x;
				if (!Map::isItSolid(tempReqt)) {
					hitBox.y += tempDistance;
					out = 1;
				}
				else tempDistance += (-1)*(speedY / abs(speedY));
			}
		}
	}

	timerA = timerB = SDL_GetTicks();
}

void Moving_Unit::move(SDL_Event & e)
{
	if (movementsLock == 0) {
		if (noclip)
			GRAVITY_ENABLED = 0;
		else GRAVITY_ENABLED = 1;
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
