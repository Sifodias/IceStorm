#include "Moving_Unit.h"
#include "Map.h"
#include <iostream>
#include "Objects_Manager.h"
#include "Controller.h"
#include <math.h>
#include "Camera.h"

using namespace std;

bool rectEquals(SDL_Rect a, SDL_Rect b) {
	return a.x == b.x
		&& a.y == b.y
		&& a.w == b.w
		&& a.h == b.h;
}

void speedRestrainer(double& speedX, double& speedY, SDL_Rect rect) {
	rect.x += 1;
	if (speedX > 0 && (Map::isItSolid(rect) || Objects_Manager::solidIntersect(rect)))
		speedX = 0;
	rect.x -= 2;
	if (speedX < 0 && (Map::isItSolid(rect) || Objects_Manager::solidIntersect(rect)))
		speedX = 0;
	rect.x += 1;
	rect.y += 1;
	if (speedY > 0 && (Map::isItSolid(rect) || Objects_Manager::solidIntersect(rect)))
		speedY = 0;
	rect.y -= 2;
	if (speedY < 0 && (Map::isItSolid(rect) || Objects_Manager::solidIntersect(rect)))
		speedY = 0;
}

Moving_Unit::Moving_Unit(SDL_Rect hitbox_i, bool inputControlled, int cspeed, int jspeed, int gravityEnabled, int noclip_i) {
	hitBox = c_rect(hitbox_i);
	move_speed = cspeed;
	jump_speed = jspeed; gravity_affected = gravityEnabled;
	noclip = noclip_i; mainDirection = 1; jumpLock = 0; movementsLock = 0;
	speedX = 0; speedY = 0;
	timerA = timerB = SDL_GetTicks();
	isInputControlled = inputControlled;
	follow_target = false;
	target = NULL;
}

void Moving_Unit::handleMoves() {
	int out = 0;
	while (!out && movesY.size()) {
		switch (movesY.back()) {
		case -1:
		if (Controller::checkActionState("up") && !jumpLock) {
			if (gravity_affected && !noclip) {
				speedY = -jump_speed;
				jumpLock = 1;
			}
			else speedY = -move_speed;
			out = 1;
		}
		else movesY.pop_back();
		break;

		case 1:
		if (Controller::checkActionState("down")) {
			speedY = move_speed;
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
		if (Controller::checkActionState("right")) {
			speedX = move_speed;
			out = 1;
		}
		else movesX.pop_back();
		break;
		case -2:
		if (Controller::checkActionState("left")) {
			speedX = -move_speed;
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
		if (Controller::checkActionState("up")) {
			mainDirection = -1;
			out = 1;
		}
		else direction.pop_back();
		break;

		case 1:
		if (Controller::checkActionState("down")) {
			mainDirection = 1;
			out = 1;
		}
		else direction.pop_back();
		break;

		case 2:
		if (Controller::checkActionState("right")) {
			mainDirection = 2;
			out = 1;
		}
		else direction.pop_back();
		break;
		case -2:
		if (Controller::checkActionState("left")) {
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

	if (!noclip)
		speedRestrainer(speedX, speedY, hitBox.sdl());

	if (!gravity_affected || noclip) {
		jumpLock = 0;
		if (!movesY.size()) {
			speedY = 0;
		}
		return;
	}
	//else if gravity is enabled
	SDL_Rect temprect = hitBox.sdl();
	temprect.y += 1;
	if (Map::isItSolid(temprect) || Objects_Manager::solidIntersect(temprect)) {
		jumpLock = 0;

	}
}

void Moving_Unit::addMoves(SDL_Event& e) {
	if (Controller::checkAction(e, "up")) {
		if (!movesY.size())
			movesY.push_back(-1);
		else if (movesY.back() != -1)
			movesY.push_back(-1);
		if (!direction.size())
			direction.push_back(-1);
		else if (direction.back() != -1)
			direction.push_back(-1);
	}
	if (Controller::checkAction(e, "down")) {
		if (!movesY.size())
			movesY.push_back(1);
		else if (movesY.back() != 1)
			movesY.push_back(1);
		if (!direction.size())
			direction.push_back(1);
		else if (direction.back() != 1)
			direction.push_back(1);
	}
	if (Controller::checkAction(e, "right")) {
		if (!movesX.size())
			movesX.push_back(2);
		else if (movesX.back() != 2)
			movesX.push_back(2);
		if (!direction.size())
			direction.push_back(2);
		else if (direction.back() != 2)
			direction.push_back(2);
	}
	if (Controller::checkAction(e, "left")) {
		if (!movesX.size())
			movesX.push_back(-2);
		else if (movesX.back() != -2)
			movesX.push_back(-2);
		if (!direction.size())
			direction.push_back(-2);
		else if (direction.back() != -2)
			direction.push_back(-2);
	}
}

void Moving_Unit::doMoves() {
	c_rect tempReqt(hitBox);
	c_rect backup(hitBox);
	timerB = SDL_GetTicks();
	double t = (double)(timerB - timerA) * 0.001;
	if (t > 0.100) {
		t = 0;
		timerA = timerB = SDL_GetTicks();
		return;
	}
	if (noclip) {
		hitBox.x += t * speedX;
		hitBox.y += t * speedY;
		timerA = timerB;
		return;
	}
	if (speedY <= 300 && gravity_affected)
		speedY += (int)(GRAVITY * t);


	int tempDistancex = t * speedX;
	int tempDistancey = t * speedY;

	//factor technique to get rid of stutter when speedx and y different and hit a wall
	//useless when both speeds are equal
	//glitch: freeze at a distance of 1 pixel for few seconds
	double factorX = abs((double)speedX / (double)(speedX + speedY));
	double factorY = abs((double)speedY / (double)(speedY + speedX));
	factorX = factorY = 1;
	double storageX = 0;
	double storageY = 0;
	while (tempDistancex * speedX > 0 || tempDistancey * speedY > 0) {
		if ((tempDistancex < 0 && speedX > 0) || (tempDistancex > 0 && speedX < 0)) {
			tempDistancex = 0;
			speedX = 0;
		}
		if ((tempDistancey < 0 && speedY > 0) || (tempDistancey > 0 && speedY < 0)) {
			tempDistancey = 0;
			speedY = 0;
		}
		tempReqt.x = hitBox.x + tempDistancex;
		tempReqt.y = hitBox.y + tempDistancey;
		if (!Map::isItSolid(tempReqt.sdl()) && !Objects_Manager::solidIntersect(tempReqt.sdl())) {
			hitBox.x += tempDistancex;
			hitBox.y += tempDistancey;
			break;
		}
		else {
			if (speedX) {
				storageX += (-1) * (speedX / abs(speedX)) * factorX;

				if (abs(storageX) >= 1) {
					tempDistancex += (int)((storageX / abs(storageX)));
					storageX -= (storageX / abs(storageX));
				}

			}
			if (speedY) {
				storageY += (-1) * (speedY / abs(speedY)) * factorY;

				if (abs(storageY) >= 1) {
					tempDistancey += (int)((storageY / abs(storageY)));
					storageY -= (storageY / abs(storageY));
				}

			}
		}
	}

	timerA = timerB;

	if (mainDirection == 1 || mainDirection == -1) {
		if (!speedY && speedX) {
			mainDirection = 2 * abs(speedX) / speedX;
		}
	}
	if (mainDirection == 2 || mainDirection == -2) {
		if (speedY && !speedX) {
			mainDirection = 1 * abs(speedY) / speedY;
		}
	}
}

void Moving_Unit::move(SDL_Event& e) {
	if (movementsLock == 0) {
		if (isInputControlled) {
			addMoves(e);
			handleMoves();
		}
		if (follow_target) {
			updateFollow();
		}
		doMoves();
	}
	if (movementsLock == 1) {
		//movementsLock = 0;
		timerA = timerB = SDL_GetTicks();
	}
}

void Moving_Unit::lockMovements(bool lock) {
	movementsLock = lock;
}

void Moving_Unit::teleport(int x, int y) {
	hitBox.x = x;
	hitBox.y = y;

}

void Moving_Unit::setLinearMovement(int speed, int angle) {
	speedX = speed * cos(3.14 * angle / 180);
	speedY = -speed * sin(3.14 * angle / 180);
}

void Moving_Unit::setPosOnCircle(double angle) {
	hitBox.x = radius * cos(3.14 * angle / 180) + (int)std::get<0>(center);
	hitBox.y = -radius * sin(3.14 * angle / 180) + (int)std::get<1>(center);
}

void Moving_Unit::followTarget(Moving_Unit& to_follow, int speed, tuple<double, double> iCenter, double iRadius) {
	circle = true;
	center = iCenter; radius = iRadius;
	followTarget(to_follow, speed);
}

void Moving_Unit::followTarget(Moving_Unit& to_follow, int speed) {
	follow_target = true;
	target = &to_follow;
	speedX = speed;	//just so norm of speedX Y is equal to speed
}

void Moving_Unit::updateFollow() {
	double x_aim = target->hitBox.x + target->hitBox.w / 2 - hitBox.x - hitBox.w / 2;
	double y_aim = target->hitBox.y + target->hitBox.h / 2 - hitBox.y - hitBox.h / 2;
	int speed = (int)sqrt(speedX * speedX + speedY * speedY);
	if (!x_aim && !y_aim)
		return;
	double angle = 0;
	if (!y_aim && x_aim < 0)
		angle = 180;
	else if (y_aim)
		angle = -(abs(y_aim) / y_aim) * 180 * acos(x_aim / sqrt((x_aim * x_aim + y_aim * y_aim))) / 3.14;

	if (!circle)
		setLinearMovement(speed, angle);
	else {
		setPosOnCircle(angle);
		// cout << angle << endl;
	}
}

tuple<double, double> Moving_Unit::getCoord(bool relative) {
	return { hitBox.x - (relative ? Camera::getX() : 0), hitBox.y - (relative ? Camera::getY() : 0) };
}

void Moving_Unit::setCoord(tuple<double, double> coord) {
	hitBox.x = get<0>(coord);
	hitBox.y = get<1>(coord);
}