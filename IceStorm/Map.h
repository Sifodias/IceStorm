#pragma once
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <string>

#include "Paths.h"
#include "Useful_Fonctions.h"

//Grid map
#define GRID_W 120
#define GRID_H 120

class Map {
public : 
	
	~Map();
	static void loadLevel();

	static int getX();			//obtenir le nombre de x du level.txt
	static int getY();			//obtenir le nombre de y du level.txt

	static void checkMate();		//écrire dans la console la matrice actuelle

	static void loadMatrix();	//charger la matrice a partir du flux

	static std::ifstream currentLevel;	//le flux contenant le niveau
	static char **matrix;	//la matrice du niveau
	static int x;		//le nombre de x du level.txt
	static int y;		//le nombre de y du level.txt
};