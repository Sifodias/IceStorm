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

	static std::vector<SDL_Texture*> textureList;	//le tableau contenant toutes les sdl_textures
	static std::vector<SDL_Texture*> texturesListInit();	//charger toutes les textures dans le tableau a partir du txt
	static void TMInit();	//initialize le texture manager

	static C_Rect camera;
};
