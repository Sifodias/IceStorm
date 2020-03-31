#pragma once
#include <vector>
#include <SDL.h>
#include <string>


namespace Textures_Manager {
	void init();	//Loads all the images
	
	void printFrame(); //blits according to the matrix, the textures and the grid
	
	SDL_Texture* findTexture(std::string name);
	SDL_Surface* findSurface(std::string name);

	extern bool showInvisibleEnts;
};
