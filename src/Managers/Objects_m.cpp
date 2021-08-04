#include <iostream>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <map>
#include "Paths.h"
#include "Textures_m.h"

#include "Objects_m.h"
#include "Quadtree.h"
#include "Save_m.h"

using json = nlohmann::json;
using namespace quadtree;

map<int, GObject> blueprints;	// ents which cannot be placed on a map
map<int, GObject> levelEnts;	// ents of the current level
string Objects_m::levelName;

struct quadNode {
	int id;
	c_rect rect;
	bool operator==(const quadNode& rhs) const {
		return (id == rhs.id) && (rect == rhs.rect);
	}
};

Box<float> getBox(quadNode node) {
	return Box<float>(node.rect.x, node.rect.y, node.rect.w, node.rect.h);
}

auto quadEnts = Quadtree<quadNode, Box<float>(quadNode node)>(Box(-(float)INT_MAX / 2, -(float)INT_MAX / 2, (float)INT_MAX, (float)INT_MAX), getBox);

// map<int, GObject> objects;
ifstream* bpFile = NULL;
ifstream* levelFile = NULL;

auto load = [](ifstream* file, string path, map<int, GObject>& container) {
	file = loadFile(path);
	if (file == NULL) {
		std::cout << "Error: Couldn't load the file at path " << path << std::endl;
		return;
	}
	container.clear();
	Objects_m::loadEnts(file, container);
};

void Objects_m::init() {
	load(bpFile, Paths::blueprintsPath, blueprints);
}

void Objects_m::loadLevel(string name) {
	load(levelFile, Paths::levelPath + name, levelEnts);
	levelName = name;
	for (auto [id, obj] : levelEnts) {
		quadEnts.add({ id, obj.movingUnit.hitBox });
	}
}

bool Objects_m::identify(string& target, string wanted) {
	string tempTarget = target;
	if (target.size() < wanted.size()) return false;
	tempTarget.erase(wanted.size(), target.size());
	if (!tempTarget.compare(wanted)) {
		target.erase(target.begin(), target.begin() + wanted.size());
		return true;
	}
	else return false;
}

void loadSprite(GObject& obj, string meta) {
	/* does not support playOnce and alpha yet */
	istringstream iss(meta);
	string word;
	iss >> word;
	string textName = word;
	vector<int> args;

	for (int i = 0; i < 6; i++) {
		iss >> word;
		if (word == textName && !i) {
			obj.textures.setSingleFrame(meta);
			obj.textures.resource = meta;
			return;
		}

		try {
			args.push_back(stoi(word));
		}
		catch (exception& e) {
			cout << "Error: invalid texture metadata format: " << meta << endl;
		}
	}
	if (!(iss >> word))
		cout << "Error: invalid texture metadata format: " << meta << endl;

	string groupName = word;
	args.push_back(100);
	if (iss >> word) {
		try {
			args.back() = stoi(word);
		}
		catch (exception& e) {
			cout << "Error: invalid texture metadata format: " << meta << endl;
		}
	}
	if (args.size() == 6)
		obj.textures.addGroup(textName, args[0], args[1], args[2], args[3], args[4], args[5], groupName);

	else if (args.size() == 7)
		obj.textures.addGroup(textName, args[0], args[1], args[2], args[3], args[4], args[5], groupName, args[6]);

	else
		cout << "Error: unsupported texture metadata format: " << meta << endl;

	obj.textures.setCurrentGroup(groupName);
	obj.textures.resource = meta;
}

void Objects_m::loadEnts(ifstream* file, map<int, GObject>& container) {
	file->seekg(0);

	json objArray;
	*file >> objArray;
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
				loadSprite(cur, ojs["texture"]);
			}

			if (field.key() == "type")
				cur.type = ojs["type"];

			if (field.key() == "flags") {
				for (auto fn : ojs["flags"])
					cur.flags.push_back(fn);
			}

			if (field.key() == "content")
				cur.meta = ojs["content"];

			if (field.key() == "x") {
				cur.movingUnit.hitBox.x = ojs["x"];
				std::get<0>(cur.movingUnit.savedCoord) = ojs["x"];
			}

			if (field.key() == "y") {
				cur.movingUnit.hitBox.y = ojs["y"];
				std::get<1>(cur.movingUnit.savedCoord) = ojs["y"];
			}

			if (field.key() == "useMUnit")
				cur.useMUnit = ojs["useMUnit"];

			if (field.key() == "enabled") {
				cur.default_enabled = ojs["enabled"];
				cur.enabled = cur.default_enabled;
			}
		}

		container[cur.ID] = cur;
	}
}

void Objects_m::routine(SDL_Event& e) {
	for (auto& [id, obj] : levelEnts) {
		obj.routine(e);
	}
}

void Objects_m::trigger(SDL_Rect rect, bool contact) {
	rect.x -= 1; rect.y -= 1; rect.w += 2; rect.h += 2;

	for (auto& [id, obj] : levelEnts) {
		SDL_Rect tempRect = obj.movingUnit.hitBox.sdl();
		if (SDL_HasIntersection(&rect, &tempRect)) {
			if ((contact && obj.checkFlag("CONTACT")) || !contact)
				obj.trigger();
		}
	}
}

vector<int> Objects_m::getIntersections(c_rect rect) {
	std::vector<quadNode> intersections = quadEnts.query(quadtree::Box<float>{(float)rect.x, (float)rect.y,
		(float)rect.w, (float)rect.h});

	vector<int> ret;

	for (auto& node : intersections) {
		ret.push_back(node.id);
	}

	return ret;
}

bool Objects_m::solidIntersect(SDL_Rect rect) {
	for (auto& [id, obj] : levelEnts) {
		SDL_Rect tempRect = obj.movingUnit.hitBox.sdl();
		if (!obj.useMUnit)
			continue;
		if (SDL_HasIntersection(&rect, &tempRect)) {
			if (obj.checkFlag("SOLID"))
				return true;
		}
	}
	return false;
}

GObject& Objects_m::findObject(string target) {
	int temp;
	try {
		temp = stoi(target);
		return findObject(temp);
	}
	catch (exception&) {
		for (auto& [id, obj] : levelEnts) {
			if (obj.target == target)
				return obj;
		}
	}
	return levelEnts[0];
}

GObject& Objects_m::findObject(int id) {
	return levelEnts.contains(id) ? levelEnts[id] : levelEnts[0];
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

void Objects_m::fillObject(GObject& obj, string data) {
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
			/* does not support playOnce and alpha yet */
			string meta = getAndClear(data);
			loadSprite(obj, meta);
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
			obj.meta = getAndClear(data);
			continue;
		}

		cout << "Error: Unknown field in: " << data << endl;
		break;
	}
}

void Objects_m::editObject(string data) {
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
GObject& Objects_m::createObject(string data) {
	GObject new_obj;

	std::vector<int> idsVec;
	for (auto& [id, obj] : levelEnts) {
		idsVec.push_back(obj.ID);
	}
	new_obj.ID = *std::max_element(idsVec.begin(), idsVec.end()) + 1;

	fillObject(new_obj, data);

	// Might need to be parametrized
	// new_obj.levelBound = Map::levelname;
	new_obj.useMUnit = new_obj.checkFlag("DYNAMIC");

	levelEnts[new_obj.ID] = new_obj;

	return levelEnts[new_obj.ID];
}

int Objects_m::duplicate(GObject& obj) {
	GObject& new_obj = createObject("");
	int id = new_obj.ID;
	new_obj = obj; new_obj.ID = id;
	return new_obj.ID;
}


void Objects_m::saveObjects(string path, map<int, GObject>& ents) {
	// if (entsFile == NULL) return;
	// entsFile->close();

	std::ofstream ojs;
	ojs.open(path, std::ofstream::out | std::ofstream::trunc);

	json objArray = json::array();

	for (auto& [id, obj] : ents) {
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

		if (!obj.meta.empty())
			curOb["content"] = obj.meta;

		if (!obj.textures.resource.empty())
			curOb["texture"] = obj.textures.resource;

		if (obj.useMUnit) {
			curOb["useMUnit"] = true;
			curOb["x"] = std::get<0>(obj.movingUnit.savedCoord);
			curOb["y"] = std::get<1>(obj.movingUnit.savedCoord);
		}

		if (!obj.default_enabled) {
			curOb["enabled"] = false;
		}

		// If the object is useless, do not store it
		if (curOb.size() == 2 && obj.ID != 0)
			objArray.erase(objArray.size() - 1);
	}

	ojs << std::setw(4) << objArray << std::endl;
	ojs.close();
}

GObject& Objects_m::getObject(int id) {
	return levelEnts[id];
}

tuple<int, int> Objects_m::newDoors(string levelName) {
	GObject& dest = createObject("texture: A2.png, type: DOOR, flags: INV, content: 1");
	GObject& from = createObject("texture: A.png, type: DOOR, flags: CONTACT INV PERMANENT, content: 0 " + to_string(dest.ID) + " " + levelName);
	return { from.ID, dest.ID };
}

void Objects_m::deleteObject(int id) {
	quadEnts.remove(quadNode{ id, levelEnts[id].movingUnit.hitBox });
	levelEnts.erase(id);
}