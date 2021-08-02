#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "Global_Flags.h"
#include <SDL2/SDL.h>
#include "Quadtree.h"
#include "Moving_Unit.h"


namespace Map {



	void loadLevel(std::string name);			//Load the stream from levelpath
	void loadMatrix();	//loads a matrix from a stream
	bool isItSolid(SDL_Rect reqt); //checks if the element at the given coord is solid
	void trigger(SDL_Rect reqt, int direction, bool contact);
	int getIdObject(double ay, int iy, double ax, int ix, int plan = 0);
	int getID(int ix, int iy, int plan = 0);
	void findOccurrence(int charry, double* ix, double* iy);
	void saveMatrix();
	void checkMate(int plan);


	extern std::vector<std::vector<std::vector<int>>> matrix;	//the matrix of the level	|  the pair indexes represent front plans, the odds back plans
	extern std::string levelname;
	// extern quadtree::Quadtree<mapNode, decltype(getBox)>* quadTest;
	// quadtree::Quadtree<mapNode, decltype(getBox)> quadTest(quadtree::Box(0.0f, 0.0f, 1.0f, 1.0f), Map::getBox);
};