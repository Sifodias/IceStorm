#include "Character.h"
#include "Map.h"
#include "Textures_Manager.h"
#include "Engine_Manager.h"
#include "Paths.h"
#include "Objects_Manager.h"

SpritesHandler Character::textures;
Moving_Unit Character::movingUnit;
ifstream* Character::saveFile = NULL;
string Character::charaMark;
string Character::map;

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
	textures.init("left.png | right.png | up.png | down.png", "left right up down");

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
			map = buffer;
			goto next;
		}

		if (Objects_Manager::identify(buffer, "charaMark: ")) {
			GObject* tempObj = Objects_Manager::findObject(buffer);
			movingUnit.teleport(tempObj->x, tempObj->y);
			charaMark = buffer;
			goto next;
		}

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
