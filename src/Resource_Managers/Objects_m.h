#pragma once
#include "Generic_Object.h"
#include <vector>
#include <map>
#include <tuple>

using namespace std;
#define ob(...) Objects_m::findObject(__VA_ARGS__)


namespace Objects_m {
	void init();

	GObject& findObject(string target);
	GObject& findObject(int id);

	bool identify(string& target, string wanted);
	void loadObjects();
	void saveObjects();
	GObject& createObject(string data);
	void fillObject(GObject& obj, string data);
	void editObject(string data);
	void deleteObject(int id);
	GObject& getObject(int index);
	int duplicate(GObject& obj);

	tuple<int, int> newDoors(string levelName);
	void objectsRoutine(SDL_Event& e);

	void trigger(SDL_Rect rect, bool contact);
	bool solidIntersect(SDL_Rect rect);

	extern vector<GObject> objects;
};






/*
Data structure of the object database :
ID: <int ID>
target: <string target>
targetnames: <strings targetnames>
texture ID: <int texture id>
type: <string type>
flags: <strings flags>
crect: <h, w>
meta: <string meta>

Example :

ID: 0
target: wall_test
targetnames: wall_test
texture: pannel.png
type: BUTTON
flags: SOLID
crect: 20, 20
meta: Vive la bite !
*/