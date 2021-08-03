#include "Character.h"
#include "Map.h"
#include "Textures_Manager.h"
#include "Paths.h"
#include "Objects_Manager.h"
#include "Controller.h"
#include "Text_Printer.h"

SpritesHandler Character::textures;
Moving_Unit Character::movingUnit;
bool Character::useMainOffsets;


string charaMark;
bool movementLocked = false;


void Character::lockMovements(bool lock) {
	Character::movingUnit.lockMovements(lock);
	textures.setIdle(lock);
	movementLocked = lock;
}

void Character::characterRoutine(SDL_Event& e) {
	lockMovements(Text_Printer::queue.size() > 0);

	movingUnit.move(e);
	if (!movementLocked) {
		switch (movingUnit.mainDirection) {
		case -2:
		textures.setCurrentGroup("left");
		textures.setIdle(movingUnit.speedX == 0);
		break;
		case 2:
		textures.setCurrentGroup("right");
		textures.setIdle(movingUnit.speedX == 0);
		break;
		case -1:
		textures.setCurrentGroup("up");
		textures.setIdle(movingUnit.speedY == 0);
		break;

		case 1:
		textures.setCurrentGroup("down");
		textures.setIdle(movingUnit.speedY == 0);
		break;
		}
	}
	if (Controller::checkAction(e, "use")) {
		Map::trigger(movingUnit.hitBox.sdl(), movingUnit.mainDirection, false);
		Objects_Manager::trigger(movingUnit.hitBox.sdl(), false);
	}
	else {
		Map::trigger(movingUnit.hitBox.sdl(), movingUnit.mainDirection, true);
		Objects_Manager::trigger(movingUnit.hitBox.sdl(), true);
	}

}

void Character::init() {
	SDL_Rect hitty = { 0, 0, CHAR_HITBOX_W, CHAR_HITBOX_H };
	useMainOffsets = true;

	movingUnit = Moving_Unit(hitty, true);
	textures.addGroup("frisk.png", 19, 29, 5, 21, 0, 4, "down", 250);
	textures.addGroup("frisk.png", 17, 29, 5, 21, 2, 2, "left", 250);
	textures.addGroup("frisk.png", 17, 29, 5, 21, 4, 2, "right", 250);
	textures.addGroup("frisk.png", 19, 29, 5, 21, 6, 4, "up", 250);


}


