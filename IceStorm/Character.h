#pragma once
#include "C_Rect.h"
#include <vector>
#include <SDL.h>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"


#define CSPEED 150

#define JSPEED 350	//jump speed
#define GRAVITY 1300
#define JUMPLOCK 1
#define GRAVITY_ENABLED 1

#define CHAR_H 30
#define CHAR_W 20
#define CHAR_HITBOX_H 30
#define CHAR_HITBOX_W 20


class Character
{
public:
	static void characterRoutine(SDL_Event& e);
	static void Init();
	
	static SpritesHandler textures;
	static Moving_Unit movingUnit;
};