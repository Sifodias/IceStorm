#include "Character.h"
#include <iostream>
#include "Map.h"
#include "Textures_Manager.h"

SpritesHandler Character::textures;
Moving_Unit Character::movingUnit;


void Character::characterRoutine(SDL_Event & e)
{
	movingUnit.move(e);
	switch (movingUnit.mainDirection) {
	case -2:
		textures.setCurrentGroup("left");
		break;
	case 2:
		textures.setCurrentGroup("right");
		break;
	case -1:
		textures.setCurrentGroup("up");
		break;

	case 1:
		textures.setCurrentGroup("down");
		break;
	}

	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_j) {
			Map::trigger(movingUnit.hitBox, movingUnit.mainDirection, false);
		}
	}
	else
		Map::trigger(movingUnit.hitBox, movingUnit.mainDirection, true);

}

void Character::Init()
{
	SDL_Rect hitty = { 0, 0, CHAR_HITBOX_W, CHAR_HITBOX_H };
	movingUnit.init(hitty);
	Map::findOccurrence(69, &movingUnit.hitBox.x, &movingUnit.hitBox.y);
	textures.init("left.png | right.png | up.png | down.png", "left right up down");
}
