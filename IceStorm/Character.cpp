#include "Character.h"
#include "Map.h"
#include "Textures_Manager.h"
#include "Engine_Manager.h"
#include "Paths.h"
#include "Objects_Manager.h"

SpritesHandler Character::textures;
Moving_Unit Character::movingUnit;

ifstream* saveFile = NULL;
string charaMark;
string map_name;

void Character::characterRoutine(SDL_Event & e)
{
	movingUnit.move(e);
	switch (movingUnit.mainDirection) {
	case -2:
		if (movingUnit.speedX)
			textures.setCurrentGroup("left");
		else textures.setCurrentGroup("sleft");
		break;
	case 2:
		if (movingUnit.speedX)
			textures.setCurrentGroup("right");
		else textures.setCurrentGroup("sright");
		break;
	case -1:
		if (movingUnit.speedY)
			textures.setCurrentGroup("up");
		else textures.setCurrentGroup("sup");
		break;

	case 1:
		if (movingUnit.speedY)
			textures.setCurrentGroup("down");
		else textures.setCurrentGroup("sdown");
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

void Character::init()
{
	SDL_Rect hitty = { 0, 0, CHAR_HITBOX_W, CHAR_HITBOX_H };
	movingUnit = Moving_Unit(hitty);
	textures = SpritesHandler("mainleft.png mainleft1.png mainleft.png mainleft2.png | mainright.png mainright1.png mainright.png mainright2.png | mainup.png mainup1.png mainup.png mainup2.png | maindown.png maindown1.png maindown.png maindown2.png | mainleft.png | mainright.png | mainup.png | maindown.png", "left right up down sleft sright sup sdown", 250);

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
