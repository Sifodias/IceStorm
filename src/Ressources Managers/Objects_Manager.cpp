#include "Objects_Manager.h"
#include <sstream>
#include <ostream>
#include <iostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <iomanip>
using json = nlohmann::json;

vector<GObject> Objects_Manager::objects;
ifstream* tempStream = NULL;

void Objects_Manager::init() {
	tempStream = loadFile(Paths::entData);
	objects.clear();
	if (tempStream == NULL) {
		printf("Error: Couldn't load data objects file\n");
		return;
	}
	loadObjects();
}

bool Objects_Manager::identify(string& target, string wanted) {
	string tempTarget = target;
	if (target.size() < wanted.size()) return false;
	tempTarget.erase(wanted.size(), target.size());
	if (!tempTarget.compare(wanted)) {
		target.erase(target.begin(), target.begin() + wanted.size());
		return true;
	}
	else return false;
}

void Objects_Manager::loadObjects() {
	tempStream->seekg(0);

	json objArray;
	*tempStream >> objArray;
	for (auto& ojs : objArray) {
		GObject cur;

		for (auto field : ojs.items()) {
			if (field.key() == "ID")
				cur.ID = ojs["ID"];

			if (field.key() == "target")
				cur.target = ojs["target"];

			if (field.key() == "targetnames") {
				for (auto tn : ojs["targetnames"])
					cur.targetnames.push_back(tn);
			}

			if (field.key() == "texture") {
				cur.textureName = ojs["texture"];
				cur.imgIndex = Textures_Manager::findIndex(cur.textureName);
				cur.useSpritesHandler = true;
				cur.textures.setSingleFrame(cur.textureName);
			}

			if (field.key() == "type")
				cur.type = ojs["type"];

			if (field.key() == "flags") {
				for (auto fn : ojs["flags"])
					cur.flags.push_back(fn);
			}

			if (field.key() == "content")
				cur.content = ojs["content"];
			
			if (field.key() == "x")
				cur.x = ojs["x"];

			if (field.key() == "y")
				cur.y = ojs["y"];
		}

		objects.push_back(cur);
	}
}

void Objects_Manager::objectsRoutine(SDL_Event& e) {
	for (int i = 0; i < objects.size(); i++) {
		objects[i].routine(e);
	}
}

void Objects_Manager::trigger(SDL_Rect rect, bool contact) {
	for (GObject& obj : objects) {
		SDL_Rect tempRect = obj.movingUnit.hitBox.sdl();
		if (SDL_HasIntersection(&rect, &tempRect)) {
			if (contact && obj.checkFlag("CONTACT") || !contact)
				obj.trigger();
		}
	}
}

bool Objects_Manager::solidIntersect(SDL_Rect rect) {
	for (GObject& obj : objects) {
		SDL_Rect tempRect = obj.movingUnit.hitBox.sdl();
		if (SDL_HasIntersection(&rect, &tempRect)) {
			if (obj.checkFlag("SOLID"))
				return true;
		}
	}
	return false;
}

GObject& Objects_Manager::findObject(string target) {
	int temp;
	try {
		temp = stoi(target);
		return findObject(temp);
	}
	catch (exception&) {
		for (GObject& obj : objects) {
			if (obj.target == target)
				return obj;
		}
	}
	return objects[0];
}

GObject& Objects_Manager::findObject(int id) {
	for (GObject& obj : objects) {
		if (obj.ID == id)
			return obj;
	}
	return objects[0];
}

void cleanSpaces(string& str) {
	while (str[0] == ' ') {
		str.erase(0, 1);
	}
}

string getAndClear(string& str) {
	cleanSpaces(str);
	string ret = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == ',') {
			str.erase(0, i + 1);
			return ret;
		}
		ret = ret + str[i];
	}
	ret = str;
	str.clear();
	return ret;
}

void Objects_Manager::fillObject(GObject& obj, string data) {
	while (!data.empty()) {
		cleanSpaces(data);

		if (identify(data, "target: ")) {
			obj.target = getAndClear(data);
			continue;
		}
		if (identify(data, "targetnames: ")) {
			istringstream iss(getAndClear(data));
			string word;
			obj.targetnames.clear();
			while (iss >> word) {
				obj.targetnames.push_back(word);
			}
			continue;
		}
		if (identify(data, "texture: ")) {
			obj.textureName = getAndClear(data);
			obj.imgIndex = Textures_Manager::findIndex(obj.textureName);
			obj.useSpritesHandler = true;
			obj.textures.setSingleFrame(obj.textureName);
			continue;
		}

		if (identify(data, "type: ")) {
			obj.type = getAndClear(data);
			continue;
		}
		if (identify(data, "flags: ")) {
			istringstream iss(getAndClear(data));
			string word;
			obj.flags.clear();
			while (iss >> word) {
				obj.flags.push_back(word);
			}
			continue;
		}
		if (identify(data, "crect: ")) {
			continue;
		}
		if (identify(data, "content: ")) {
			obj.content = getAndClear(data);
			continue;
		}

		cout << "Error: Unknown field in: " << data << endl;
		break;
	}
}

void Objects_Manager::editObject(string data) {
	cleanSpaces(data);
	string target = "";
	for (int i = 0; i < data.size(); i++) {
		if (data[i] != ' ')
			target = target + data[i];
		else break;
	}
	data.erase(0, target.size() + 1);

	if (findObject(target).ID)
		fillObject(findObject(target), data);
}

//syntax in console : new ent <field1>: <value1>, <field2>: <value2> ...
GObject& Objects_Manager::createObject(string data) {
	GObject new_obj;

	std::vector<int> idsVec;
	for (GObject& obj : objects) {
		idsVec.push_back(obj.ID);
	}
	new_obj.ID = *std::max_element(idsVec.begin(), idsVec.end()) + 1;

	fillObject(new_obj, data);

	objects.push_back(new_obj);

	return objects.back();
}

void Objects_Manager::saveObjects() {
	if (tempStream == NULL) return;
	tempStream->close();

	std::ofstream ojs;
	ojs.open(Paths::entData, std::ofstream::out | std::ofstream::trunc);

	json objArray = json::array();

	for (GObject& obj : objects) {
		if (obj.checkFlag("DYNAMIC"))
			continue;
		objArray.push_back(json::object());
		auto& curOb = objArray.back();

		curOb["ID"] = obj.ID;

		if (!obj.target.empty())
			curOb["target"] = obj.target;

		if (!obj.targetnames.empty()) {
			curOb["targetnames"] = json::array();
			for (int j = 0; j < obj.targetnames.size(); j++) {
				curOb["targetnames"].push_back(obj.targetnames[j]);
			}
		}

		if (!obj.flags.empty()) {
			curOb["flags"] = json::array();
			for (int j = 0; j < obj.flags.size(); j++) {
				curOb["flags"].push_back(obj.flags[j]);
			}
		}

		curOb["type"] = obj.type;

		if (!obj.textureName.empty())
			curOb["texture"] = obj.textureName;

		if (!obj.content.empty())
			curOb["content"] = obj.content;
		
		if (obj.x != 0)
			curOb["x"] = obj.x;
		
		if (obj.y != 0)
			curOb["y"] = obj.y;
			
		// If the object is useless, do not store it
		if (curOb.size() == 2 && obj.ID != 0)
			objArray.erase(objArray.size()-1);
	}

	ojs << std::setw(4) << objArray << std::endl;
	ojs.close();
}

GObject& Objects_Manager::getObject(int index) {
	if (index < 0 || index >= objects.size()){
		std::cout << "Error: Invalid index " << index << std::endl;
		return objects[0];
	}
	return objects[index];
}

tuple<int, int> Objects_Manager::newDoors(string levelName) {
	GObject& dest = createObject("texture: A2.png, type: DOOR, flags: INV, content: 1");
	GObject& from = createObject("texture: A.png, type: DOOR, flags: CONTACT INV, content: 0 " + to_string(dest.ID) + " " + levelName);
	return { from.ID, dest.ID };
}

void Objects_Manager::deleteObject(int id) {
	int i = 0;
	for (GObject& obj : objects) {
		if (obj.ID == id) {
			objects.erase(objects.begin() + i);
			return;
		}
		i++;
	}
}