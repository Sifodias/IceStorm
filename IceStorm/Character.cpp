#include "Character.h"
#include <iostream>
#include "Map.h"
#include "Textures_Manager.h"

//static definitions
SDL_Texture* Character::texture = NULL;
C_Rect Character::hitBox;
double Character::speedX = 0;
double Character::speedY = 0;
std::vector<int> Character::movesX;
std::vector<int> Character::movesY;
int Character::mainDirection = 2;
std::vector<int> Character::direction;
Uint32 Character::timerA = SDL_GetTicks();
Uint32 Character::timerB = SDL_GetTicks();
//
void Character::handleMoves()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	int out = 0;
	while (!out && movesY.size()) {
		switch (movesY.back()) {
		case -1:
			if (state[SDL_SCANCODE_W]) {
				speedY = -CSPEED;
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
	if (!movesY.size()) {
		speedY = 0;
	}
}

void Character::addMoves(SDL_Event & e)
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

void Character::doMoves()
{
	C_Rect tempReqt(hitBox);
	C_Rect backup(hitBox);
	timerB = SDL_GetTicks();
	double t = (double)(timerB - timerA)*0.001;
	int out = 0;
	if (!speedY) {
		double tempDistance = t*speedX;
		while (!out && tempDistance != 0) {
			tempReqt.x = hitBox.x + tempDistance;
			if (!Map::isItSolid(tempReqt)) {
				hitBox.x += tempDistance;
				out = 1;
			}
			else {
				tempDistance += (-1)*(speedX / abs(speedX));
			}
			if ((tempDistance > 0 && speedX < 0) || (tempDistance < 0 && speedX > 0)) {
				out = 1;
			}
		}
	}
	else if (!speedX) {
		double tempDistance = t*speedY;
		while (!out && tempDistance != 0) {
			tempReqt.y = hitBox.y + tempDistance;
			if (!Map::isItSolid(tempReqt)) {
				hitBox.y += tempDistance;
				out = 1;
			}
			else {
				tempDistance += (-1)*(speedY / abs(speedY));
			}
			if ((tempDistance > 0 && speedY < 0) || (tempDistance < 0 && speedY > 0)) {
				out = 1;
			}
		}
	}
	else {
		double tempDistanceY = t*speedY;
		double tempDistanceX = t*speedX;
		while (!out && tempDistanceX != 0 && tempDistanceY != 0) {
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
			if ((tempDistanceY > 0 && speedY < 0) || (tempDistanceY < 0 && speedY > 0)) {
				out = 1;
			}
		}
		if (backup == hitBox) {
			out = 0;
			double tempDistance = t*speedX;
			while (!out && tempDistance != 0) {
				tempReqt.x = hitBox.x + tempDistance;
				tempReqt.y = hitBox.y;
				if (!Map::isItSolid(tempReqt)) {
					hitBox.x += tempDistance;
					out = 1;
				}
				else {
					tempDistance += (-1)*(speedX / abs(speedX));
				}
				if ((tempDistance > 0 && speedX < 0) || (tempDistance < 0 && speedX > 0)) {
					out = 1;
				}
			}

			tempDistance = t*speedY;
			out = 0;
			while (!out && tempDistance != 0) {
				tempReqt.y = hitBox.y + tempDistance;
				tempReqt.x = hitBox.x;
				if (!Map::isItSolid(tempReqt)) {
					hitBox.y += tempDistance;
					out = 1;
				}
				else {
					tempDistance += (-1)*(speedY / abs(speedY));
				}
				if ((tempDistance > 0 && speedY < 0) || (tempDistance < 0 && speedY > 0)) {
					out = 1;
				}
			}
		}
	}

	timerA = timerB = SDL_GetTicks();
}

void Character::move(SDL_Event & e)
{
	addMoves(e);
	handleMoves();
	doMoves();
}

void Character::initialize()
{
	hitBox.h = CHAR_H;
	hitBox.w = CHAR_W;
	Map::findOccurrence(69, &hitBox.x, &hitBox.y);
	texture = Textures_Manager::loadTexture("./Textures/testc.png");
}
