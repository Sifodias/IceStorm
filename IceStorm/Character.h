#pragma once
#include <SDL.h>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"
#include <fstream>

class Character
{
	//save fields
	static string charaMark;
	static string map;
	//

public:
	static void characterRoutine(SDL_Event& e);
	static void Init();

	static SpritesHandler textures;
	static Moving_Unit movingUnit;

	static void loadSave();// string save = "save.txt");
	static void save();// string save = "save.txt");

	static ifstream* saveFile;

};