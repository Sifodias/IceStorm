#pragma once
#include "Objects_Manager.h"
#include <sstream>
#include <ostream>
#include <iostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"
#include <algorithm>

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

bool Objects_Manager::identify(string& target, string wanted)
{
	string tempTarget = target;
	if (target.size() < wanted.size()) return false;
	tempTarget.erase(wanted.size(), target.size());
	if (!tempTarget.compare(wanted)) {
		target.erase(target.begin(), target.begin() + wanted.size());
		return true;
	}
	else return false;
}

void Objects_Manager::loadObjects()
{
	std::string buffer;
	tempStream->seekg(0);
	std::getline(*tempStream, buffer);


	while (buffer.compare("EOF")) {
		while (!buffer.size()) {
			std::getline(*tempStream, buffer);
			continue;
		}
		identify(buffer, "ID: ");
		GObject currentObject;
		currentObject.ID = std::stoi(buffer);

		std::getline(*tempStream, buffer);
		while (!identify(buffer, "ID: ") && buffer.compare("EOF")) {
			//fill the element
			if (identify(buffer, "target: ")) {
				currentObject.target = buffer;
				if (findObject(buffer).ID)
					std::cout << "Objects with the same target : " << buffer <<
					". Can result in unexpected behavior" << std::endl;

				goto next;
			}
			if (identify(buffer, "targetnames: ")) {
				istringstream iss(buffer);
				string word;
				while (iss >> word) {
					currentObject.targetnames.push_back(word);
				}
				goto next;
			}
			if (identify(buffer, "texture: ")) {
				currentObject.textureName = buffer;
				currentObject.imgIndex = Textures_Manager::findIndex(currentObject.textureName);
				currentObject.useSpritesHandler = true;
				currentObject.textures.setSingleFrame(currentObject.textureName);
				goto next;
			}

			if (identify(buffer, "type: ")) {
				currentObject.type = buffer;
				goto next;
			}
			if (identify(buffer, "flags: ")) {
				istringstream iss(buffer);
				string word;
				while (iss >> word) {
					currentObject.flags.push_back(word);
				}
				goto next;
			}
			if (identify(buffer, "crect: ")) {
				goto next;
			}
			if (identify(buffer, "content: ")) {
				currentObject.content = buffer;
				goto next;
			}

		next:
			std::getline(*tempStream, buffer);
		}
		objects.push_back(currentObject);
	}
}

void Objects_Manager::objectsRoutine(SDL_Event& e)
{
	for (int i = 0; i < objects.size(); i++) {
		objects[i].routine(e);
	}
}

GObject& Objects_Manager::findObject(string target) {
	int temp;
	try {
		temp = stoi(target);
		for (GObject& obj : objects) {
			if (obj.ID == temp)
				return obj;
		}
		return objects[0];
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

/* TODO: Remove code duplication */
void Objects_Manager::fillObject(GObject& obj, string data) {
	while (!data.empty()) {
		cleanSpaces(data);

		if (identify(data, "target: ")) {
			obj.target = getAndClear(data);
			if (findObject(obj.target).ID)
				std::cout << "Objects with the same target : " << obj.target <<
				". Can result in unexpected behavior" << std::endl;
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
	for( GObject& obj : objects) {
		idsVec.push_back(obj.ID);
	}
	new_obj.ID = *std::max_element(idsVec.begin(), idsVec.end())+1;

	fillObject(new_obj, data);

	objects.push_back(new_obj);

	return objects.back();
}



//beware of new fields
void Objects_Manager::saveObjects() {
	if (tempStream == NULL) return;
	tempStream->close();
	std::ofstream ofs;
	ofs.open(Paths::entData, std::ofstream::out | std::ofstream::trunc);

	for (GObject& obj : objects) {
		if (obj.checkFlag("DYNAMIC"))
			continue;
		ofs << "ID: " << obj.ID << endl;

		if (!obj.target.empty())
			ofs << "target: " << obj.target << endl;

		if (!obj.targetnames.empty()) {
			ofs << "targetnames:";
			for (int j = 0; j < obj.targetnames.size(); j++)
				ofs << " " << obj.targetnames[j];
			ofs << endl;
		}

		if (!obj.flags.empty()) {
			ofs << "flags:";
			for (int j = 0; j < obj.flags.size(); j++)
				ofs << " " << obj.flags[j];
			ofs << endl;
		}

		ofs << "type: " << obj.type << endl;
		
		if (!obj.textureName.empty())
			ofs << "texture: " << obj.textureName << endl;

		if (!obj.content.empty())
			ofs << "content: " << obj.content << endl;
		ofs << endl << endl;
	}

	ofs << endl << endl << "EOF";
	ofs.close();

}

tuple<GObject, GObject> Objects_Manager::newDoors(string levelName) {
	GObject& dest = createObject("texture: A2.png, type: DOOR, flags: INV, content: 1");
	GObject& from = createObject("texture: A.png, type: DOOR, flags: CONTACT INV, content: 0 " + to_string(dest.ID) + " " +  levelName);
	return { from, dest };
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