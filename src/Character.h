#pragma once
#include <SDL2/SDL.h>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"
#include <fstream>

namespace Character
{
	void init();
	void characterRoutine(SDL_Event& e);
	void lockMovements(bool lock);

	void loadSave();// string save = "save.txt");	//Move it somewhere else
	void save();// string save = "save.txt");		//same

	extern SpritesHandler textures;
	extern Moving_Unit movingUnit;
	

	extern bool useMainOffsets;	// if not, hitbox dim & blit will be according to surf dimensions
};