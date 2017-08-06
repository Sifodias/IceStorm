#pragma once

#include <fstream>
#include <SDL.h>
#include <vector>
#include <string>
#include "C_Rect.h"

#include "Global_Flags.h"


class Map {
public : 
	static void loadLevel();

	static void checkMate();		//prints in the console the current loaded matrix

	static void loadMatrix();	//loads a matrix from a stream

	static bool isItSolid(C_Rect reqt); //checks if the element at the given coord is solid

	static void trigger(C_Rect reqt, int direction);

	static int getIdObject(double ay, int iy, double ax, int ix);

	static void findOccurrence(int charry, double* ix, double* iy);

	static void saveMatrix();

	static bool changed;
	static std::ifstream currentLevel;	//the stream containing the level
	static std::vector<std::vector<int>> matrix;	//the matrix of the level

	static int cx;		//the x of the matrix in the stream currentlevel
	static int cy;
};