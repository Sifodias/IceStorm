#pragma once

#include "Generic_Object.h"
#include <vector>

using namespace std;
class Builder {
public:
	static void fetch();
	static void zoom(int focus);
	static void trace(int set, int plan = 0);
	static void nearby();
	static void createObject(string buffer);
	static void editObject(string target);
	static void placeElement(int x, int y, int plan = 0);
	static void saveLevel(std::string name);
	static void newLevel(std::string name);
	static void loadLevel(std::string name);
	static void printInfo(GObject* printObject);
	static void routine(SDL_Event& e);
	static bool checkKey(int key);
	static void setKey(int key);
	static void loadEnts();
	static void newPlan(int plan);
	static void setPlan(int plan);
	static void clean(int plan); //remove the 0 useless in the matrix

	static GObject* currentObject;
	static int currentPlan;
	static vector<int> cmdDone;

};
