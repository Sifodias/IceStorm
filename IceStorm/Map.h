#pragma once
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <string>
#include <vector>
#include "Paths.h"
#include "Useful_Fonctions.h"
#include "C_Rect.h"
//Grid map
#define GRID_W 20
#define GRID_H 20

class Map {
public : 
	
	~Map();
	static void loadLevel();

	static int getX();			//gets the x of the matrix in the stream currentlevel
	static int getY();			//same for y

	static void checkMate();		//prints in the console the current loaded matrix

	static void loadMatrix();	//loads a matrix from a stream

	static bool isItSolid(C_Rect reqt); //checks if the element at the given coord is solid

	static void findOccurrence(int charry, double* ix, double* iy);

	static std::ifstream currentLevel;	//the stream containing the level
	static std::vector<std::vector<int>> matrix;	//the matrix of the level
	static int x;		//the x of the matrix in the stream currentlevel
	static int y;		//same for y
};