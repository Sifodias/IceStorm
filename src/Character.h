#pragma once
#include <SDL2/SDL.h>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"
#include <fstream>

namespace Character
{
	void init();
	void characterRoutine(SDL_Event& e);
	void loadSave();// string save = "save.txt");
	void save();// string save = "save.txt");
	void lockMovements(bool lock);

	extern SpritesHandler textures;
	extern Moving_Unit movingUnit;
	

	extern bool useMainOffsets;	// if not, hitbox dim & blit will be according to surf dimensions
};