#pragma once
//trace
//nearby
//event place mouse
//set element to place
//create an element to the database
//save the level
//create a new level
#include "Character.h"
#include "Generic_Object.h"
using namespace std;
class Builder {
public:
	static void fetch();
	static void trace();
	static void nearby();
	static void createObject();
	static void placeElement(SDL_MouseButtonEvent& e);
	static void saveLevel(std::string name);
	static void newLevel();
	static void loadLevel(std::string name);
	static void printInfo(GObject* printObject);
	static void routine(SDL_Event& e);

	static GObject* currentObject;

};
