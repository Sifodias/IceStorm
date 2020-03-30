#pragma once
#include <SDL.h>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"
#include <fstream>

namespace Character
{
	void init();
	void characterRoutine(SDL_Event& e);
	void loadSave();// string save = "save.txt");
	void save();// string save = "save.txt");

	extern SpritesHandler textures;
	extern Moving_Unit movingUnit;
};