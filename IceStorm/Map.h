#pragma once
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <string>

#include "Paths.h"
#include "Useful_Fonctions.h"

//Grid map
#define GRID_W 24
#define GRID_H 24

class Map {
public : 
	
	~Map();
	static void loadLevel();

	static int getX();			//gets the x of the matrix in the stream currentlevel
	static int getY();			//same for y

	static void checkMate();		//prints in the console the current loaded matrix

	static void loadMatrix();	//loads a matrix from a stream

	static std::ifstream currentLevel;	//the stream containing the level
	static char **matrix;	//the matrix of the level
	static int x;		//the x of the matrix in the stream currentlevel
	static int y;		//same for y
};