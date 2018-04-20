#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "Global_Flags.h"
#include <SDL.h>

class Map {
public : 
	static void loadLevel(std::string name);			//Load the stream from levelpath

	static void loadMatrix();	//loads a matrix from a stream

	static bool isItSolid(SDL_Rect reqt); //checks if the element at the given coord is solid

	static void trigger(SDL_Rect reqt, int direction, bool contact);

	static int getIdObject(double ay, int iy, double ax, int ix, int plan = 0);
	
	static int getID(int ix, int iy, int plan = 0);

	static void findOccurrence(int charry, int* ix, int* iy);

	static void saveMatrix();
	static void checkMate(int plan);
	static bool changed;
	static std::ifstream* currentLevel;	//the stream containing the level
	static std::vector<std::vector<std::vector<int>>> matrix;	//the matrix of the level	|  the pair indexes represent front plans, the odds back plans
	static std::string levelname;


};