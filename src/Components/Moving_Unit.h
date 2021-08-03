#pragma once
#include <vector>
#include "Global_Flags.h"
#include <SDL2/SDL.h>
#include <tuple>
#include "Box.h"

class c_rect {
public:
	c_rect(double x, double y, int w, int h) : x(x), y(y), w(w), h(h) {}
	c_rect() : x(0), y(0), w(0), h(0) {}
	c_rect(SDL_Rect& rect) {
		x = rect.x; y = rect.y;
		w = rect.w; h = rect.h;
	}
	SDL_Rect sdl() {	// BEWARE : it returns a copy
		return { (int)x, (int)y, w, h };
	}
	quadtree::Box<float> box() {
		return quadtree::Box<float>((float)x, (float)y, (float)w, (float)h);
	}
	double x;
	double y;
	int w;
	int h;
	bool operator==(const c_rect& rhs) const {
		return (w == rhs.w) && (h == rhs.h)
			&& (abs(x - rhs.x) < 0.000000001) && (abs(y - rhs.y) < 0.000000001);
	}
};

class Moving_Unit {
public:
	Moving_Unit(SDL_Rect hitbox_i = SDL_Rect{ 0, 0, GRID_W, GRID_H }, bool inputControlled = false, int cspeed = CSPEED, int jspeed = JSPEED,
		int gravityEnabled = GRAVITY_ENABLED, int noclip_i = 0);

	void move(SDL_Event& e);
	void lockMovements(bool lock);
	void teleport(int x, int y);
	void setLinearMovement(int speed, int angle);
	void followTarget(Moving_Unit& to_follow, int speed, tuple<double, double> center, double radius);
	void followTarget(Moving_Unit& to_follow, int speed);
	tuple<double, double> getCoord(bool relative = false);
	void setCoord(tuple<double, double> coord);
	// c_rect getBox();
	c_rect hitBox;

	double speedX;
	double speedY;

	std::vector<int> movesX;
	std::vector<int> movesY;
	int mainDirection;
	tuple<double, double> savedCoord;
	std::vector<int> direction;		//1 = down	| -1 = up
									//2 = right | -2 = left
	int jump_speed;
	int move_speed;

	Uint32 timerA;
	Uint32 timerB;
	bool jumpLock;
	bool noclip;
	bool gravity_affected;
	bool movementsLock;
	bool follow_target;

	bool circle;	// If true, when following target, will move only on the edge of a predefined circle
	tuple<double, double> center;
	double radius;

	Moving_Unit* target;

	bool isInputControlled;	// True if unit controlled by keyboard input

	void handleMoves();
	void addMoves(SDL_Event& e);
	void doMoves();

	void setPosOnCircle(double angle);
	void updateFollow();
};
