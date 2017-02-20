#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include "Useful_Fonctions.h"
#include <string>
#include "Renderer.h"
#include <iostream>
#include "Paths.h"
#include "C_Rect.h"
#include "Map.h"

class Textures_Manager {
public:

	static SDL_Texture* loadTexture(std::string path);
	static void blitStuff(); //blits according to the matrix, the textures and the grid
	static std::vector<SDL_Texture*> texturesListInit();	//Loads the textures from a txt containing the names of the files
	static void TMInit();	//Initializes the textures manager

	static std::vector<SDL_Texture*> textureList;	//The table containing the textures
	static C_Rect camera;
};
