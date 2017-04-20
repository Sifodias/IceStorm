#pragma once
#include "Generic_Object.h"
#include <vector>
#include <ostream>
#include "Paths.h"
#include "Engine_Manager.h"
using namespace std;

class Objects_Manager {
public:
	Objects_Manager() {
		init_OM();
	}

	void init_OM() {
		ifstream tempStream = loadFile(Paths::objectsDatabasePath);
		//
	}

	GObject* find(string target) {
		for(auto i = objects.begin(); i!= objects.end(); ++i){
			if (!i->target.compare(target))
				return &(*i);
		}
		return NULL;
	}

	static vector<GObject> objects;
};
