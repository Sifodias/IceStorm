#pragma once
#include "Generic_Object.h"
#include <vector>
#include <ostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"
#include <vector>
using namespace std;

class Objects_Manager {
public:
	Objects_Manager() {
		init_OM();
	}

	void init_OM();

	GObject* find(string target) {
		for (auto i = objects.begin(); i != objects.end(); ++i) {
			if (!(*i)->target.compare(target))
				return *i;
		}
		return NULL;
	}

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
*/