#pragma once
#include "Generic_Object.h"
#include <vector>
#include <map>
#include <tuple>

using namespace std;
#define ob(...) Objects_m::findObject(__VA_ARGS__)


namespace Objects_m {
	void init();

	vector<int> getIntersections(c_rect rect);
	GObject& findObject(string target);
	GObject& findObject(int id);

	bool identify(string& target, string wanted);
	void loadLevel(string path);
	void loadEnts(ifstream* file, map<int, GObject>& container);

	void saveObjects(string path, map<int, GObject>& ents);
	GObject& createObject(string data);
	void fillObject(GObject& obj, string data);
	void editObject(string data);
	void deleteObject(int id);
	GObject& getObject(int index);
	int duplicate(GObject& obj);

	tuple<int, int> newDoors(string levelName);
	void routine(SDL_Event& e);

	void trigger(SDL_Rect rect, bool contact);
	bool solidIntersect(SDL_Rect rect);

	// extern vector<GObject> objects;
	// extern map<int, GObject> objects;
	extern string levelName;

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