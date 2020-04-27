#include "Character.h"
#include "Map.h"
#include "Textures_Manager.h"
#include "Engine_Manager.h"
#include "Paths.h"
#include "Objects_Manager.h"

SpritesHandler Character::textures;
Moving_Unit Character::movingUnit;
bool Character::useMainOffsets;

ifstream* saveFile = NULL;
string charaMark;
string map_name;
bool movementLocked = false;


void Character::lockMovements(bool lock) {
	Character::movingUnit.lockMovements(lock);
	textures.setIdle(lock);
	movementLocked = lock;
}

void Character::characterRoutine(SDL_Event& e)
{
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
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_j) {
			Map::trigger(movingUnit.hitBox.sdl(), movingUnit.mainDirection, false);
		}
	}
	else
		Map::trigger(movingUnit.hitBox.sdl(), movingUnit.mainDirection, true);

}

void Character::init()
{
	SDL_Rect hitty = { 0, 0, CHAR_HITBOX_W, CHAR_HITBOX_H };
	useMainOffsets = true;

	movingUnit = Moving_Unit(hitty, true);
	textures.addGroup("frisk.png", 19, 29, 5, 21, 0, 4, "down", 250);
	textures.addGroup("frisk.png", 17, 29, 5, 21, 2, 2, "left", 250);
	textures.addGroup("frisk.png", 17, 29, 5, 21, 4, 2, "right", 250);
	textures.addGroup("frisk.png", 19, 29, 5, 21, 6, 4, "up", 250);

	saveFile = loadFile(Paths::saveData + "save.txt");
	loadSave();
}

void Character::loadSave()
{
	std::string buffer;
	saveFile->seekg(0);
	std::getline(*saveFile, buffer);

	while (buffer.compare("EOF")) {
		if (Objects_Manager::identify(buffer, "map: ")) {
			Map::loadLevel(buffer);
			map_name = buffer;
			goto next;
		}
		Map::findOccurrence(69, &Character::movingUnit.hitBox.x, &Character::movingUnit.hitBox.y);

		/*if (Objects_Manager::identify(buffer, "charaMark: ")) {
			GObject tempObj = Objects_Manager::findObject(buffer);
			movingUnit.teleport(tempObj.x, tempObj.y);
			charaMark = buffer;
			goto next;
		}*/

	next:
		std::getline(*saveFile, buffer);
	}
}

void Character::save()
{
	//write all the fields loaded by loadSavem in the saveFile
}

/*
Content of a save file :
map = Map where it was saved
charaMark = Last character marker



*/
