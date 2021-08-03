#pragma once
#include <SDL2/SDL.h>
#include "Moving_Unit.h"
#include "Sprite.h"
#include <fstream>

namespace Character
{
	void init();
	void characterRoutine(SDL_Event& e);
	void lockMovements(bool lock);

	

	extern Sprite textures;
	extern Moving_Unit movingUnit;
	

	extern bool useMainOffsets;	// if not, hitbox dim & blit will be according to surf dimensions
};