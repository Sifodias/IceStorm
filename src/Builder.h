#pragma once

#include "Generic_Object.h"
#include <vector>

using namespace std;
namespace Builder {
	void fetch();
	void zoom(int focus);
	void trace(int set, int plan = 0);
	void nearby();
	void createObject(string buffer);
	void editObject(string target);
	void placeElement(int x, int y, int plan = 0);
	void saveLevel(std::string name);
	void newLevel(std::string name);
	void loadLevel(std::string name);
	void printInfo(GObject& printObject);
	void routine(SDL_Event& e);
	bool checkKey(int key);
	void setKey(int key);
	void loadEnts();
	void newPlan(int plan);
	void setPlan(int plan);
	void clean();
	void newDoor(string levelname);

	extern GObject* currentObject;

};

/*
Key mapping :
t = console mode
o = zoom out
p = zoom in
i = freedom cam + noclip
r = trace
q = trace and set as current
n = checkmate
c = clean matrix from 0


Console commands:
new		level
		plan
		ent

load	ents
		level

current	ent
		plan

set		plan
		ent

info name/ID

edit name/ID field1: value1, field2: value2...

*/
