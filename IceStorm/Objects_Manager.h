#pragma once
#include "Generic_Object.h"
#include <vector>
using namespace std;

class Objects_Manager {
public:
	Objects_Manager() {
		Init();
	}

	static void Init();

	static GObject* findObject(string target);

	static bool identify(string& target, string wanted);

	static vector<GObject*> objects;
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