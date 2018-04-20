#pragma once
#include <vector>
#include <SDL.h>
#include <string>


class Textures_Manager {
public:
	static void Init();	//Inits the textures manager
	static SDL_Texture* loadTexture(std::string path);
	static void blitStuff(); //blits according to the matrix, the textures and the grid
	static std::vector<SDL_Texture*> texturesListInit();	//Loads the textures from a txt containing the names of the files
	static SDL_Texture* findTexture(std::string name);

	static std::vector<SDL_Texture*> textureList;	//The table containing the textures
	static std::vector<std::string> textureNames;
	static SDL_Texture* levelScreenshot;
	static bool showInvisibleEnts;
};
