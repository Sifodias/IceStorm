#pragma once
//trace
//nearby
//event place mouse
//set element to place
//create an element to the database
//save the level
//create a new level

#include "Generic_Object.h"
#include <vector>

using namespace std;
class Builder {
public:
	static void fetch();
	static void zoom(int focus);
	static void trace();
	static void nearby();
	static void createObject();
	static void editObject();
	static void placeElement(int x, int y);
	static void saveLevel(std::string name);
	static void newLevel(std::string name);
	static void loadLevel(std::string name);
	static void printInfo(GObject* printObject);
	static void routine(SDL_Event& e);
	static bool checkKey(int key);
	static void setKey(int key);
	static GObject* currentObject;

	static vector<int> cmdDone;

};
