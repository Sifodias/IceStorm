#pragma once
#include <vector>
#include <SDL.h>
#include <string>


namespace Textures_Manager {
	void init();	//Inits the textures manager
	SDL_Texture* loadTexture(std::string path);
	void blitStuff(); //blits according to the matrix, the textures and the grid
	std::vector<SDL_Texture*> texturesListInit();	//Loads the textures from a txt containing the names of the files
	SDL_Texture* findTexture(std::string name);
};
