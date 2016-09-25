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
	static void blitStuff();

	static std::vector<SDL_Texture*> textureList;	//The table containing the textures
	static std::vector<SDL_Texture*> texturesListInit();	//Load the textures from a txt containing the names of the files
	static void TMInit();	//Initialize the textures manager

	static C_Rect camera;
};
