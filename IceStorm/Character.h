#pragma once
#include "C_Rect.h"
#include <vector>
#include <SDL.h>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"
#include "Global_Flags.h"


class Character
{
public:
	static void characterRoutine(SDL_Event& e);
	static void Init();

	static SpritesHandler textures;
	static Moving_Unit movingUnit;
};