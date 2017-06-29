#include "Character.h"
#include <iostream>
#include "Map.h"
#include "Textures_Manager.h"

SpritesHandler Character::textures;
Moving_Unit Character::movingUnit;


void Character::characterRoutine(SDL_Event & e)
{
	movingUnit.move(e);
	if (movingUnit.mainDirection == -2) {
		textures.setCurrentGroup("left");
	}
	if (movingUnit.mainDirection == 2) {
		textures.setCurrentGroup("right");
	}
	/*if (movingUnit.direction.size() > 0) {
		if (movingUnit.direction[0] == -2) {
			textures.setCurrentGroup("left");
		}
		if (movingUnit.direction[0] == 2) {
			textures.setCurrentGroup("right");
		}
	}*/
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_j) {
			Map::trigger(movingUnit.hitBox, movingUnit.mainDirection);
		}
	}
}

void Character::Init()
{
	C_Rect hitty(0, 0, CHAR_HITBOX_W, CHAR_HITBOX_H);
	movingUnit.init(hitty);
	Map::findOccurrence(69, &movingUnit.hitBox.x, &movingUnit.hitBox.y);
	textures.init("lucasL.png | lucasR.png", "left right");
}
