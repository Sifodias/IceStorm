#pragma once
#include "Generic_Object.h"
#include <vector>
#include <map>

using namespace std;

class Objects_Manager {
public:
	static void Init();

	static GObject* findObject(string target);

	static  GObject* findObjectOfID(int id);

	static bool identify(string& target, string wanted);

	static void loadObjects();
	static void saveObjects();
	static GObject* createObject(string data);
	static void fillObject(GObject* obj, string data);
	static void editObject(string data);

	static void objectsRoutine();

	static vector<GObject*> objects;
	static ifstream* tempStream;
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