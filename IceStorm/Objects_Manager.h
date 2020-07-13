#pragma once
#include "Generic_Object.h"
#include <vector>
#include <map>
#include <tuple>
using namespace std;

namespace Objects_Manager {
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

	tuple<GObject, GObject> newDoors(string levelName);
	void objectsRoutine(SDL_Event& e);

	void trigger(SDL_Rect& rect, bool contact);
	bool solidIntersect(SDL_Rect& rect);

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
content: <string content>

Example :

ID: 0
target: wall_test
targetnames: wall_test
texture: pannel.png
type: BUTTON
flags: SOLID
crect: 20, 20
content: Vive la bite !
*/