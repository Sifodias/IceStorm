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

	static void createObject(int mode, string data = "");	//mode 0 for creating an object from the string
															//mode 1 for fetching the data from the datafile (init only)

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