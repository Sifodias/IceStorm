#pragma once
#include "Objects_Manager.h"
#include <sstream>
#include <ostream>
#include <iostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"
#include <algorithm>

vector<GObject*> Objects_Manager::objects;
ifstream* Objects_Manager::tempStream = NULL;

void Objects_Manager::Init() {
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


	GObject* currentObject;
	while (buffer.compare("EOF")) {
		while (!buffer.size()) {
			std::getline(*tempStream, buffer);
			continue;
		}
		identify(buffer, "ID: ");
		currentObject = new GObject;
		currentObject->ID = std::stoi(buffer);
		std::getline(*tempStream, buffer);
		while (!identify(buffer, "ID: ") && buffer.compare("EOF")) {
			//fill the element
			if (identify(buffer, "target: ")) {
				currentObject->target = buffer;
				if (findObject(buffer) != objects[0])
					std::cout << "Objects with the same target : " << buffer <<
					". Can result in unexpected behavior" << std::endl;
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "targetnames: ")) {
				istringstream iss(buffer);
				string word;
				while (iss >> word) {
					currentObject->targetnames.push_back(word);
				}
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "texture: ")) {
				currentObject->textureName = buffer;
				currentObject->texture = Textures_Manager::findTexture(buffer);
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "type: ")) {
				currentObject->type = buffer;
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "flags: ")) {
				istringstream iss(buffer);
				string word;
				while (iss >> word) {
					currentObject->flags.push_back(word);
				}
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "crect: ")) {
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "content: ")) {
				currentObject->content = buffer;
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "x: ")) {
				currentObject->x = std::stoi(buffer);
				currentObject->movingUnit.hitBox.x = std::stoi(buffer);
				std::getline(*tempStream, buffer);
				continue;
			}
			if (identify(buffer, "y: ")) {
				currentObject->y = std::stoi(buffer);
				currentObject->movingUnit.hitBox.y = std::stoi(buffer);
				std::getline(*tempStream, buffer);
				continue;
			}
			else std::getline(*tempStream, buffer);
		}
		objects.push_back(currentObject);
	}
}

void Objects_Manager::objectsRoutine()
{
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->routine();
	}

}

GObject* Objects_Manager::findObject(string target) {
	int temp;
	try {
		temp = stoi(target);
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->ID == temp)
				return objects[i];
		}
		return objects[0];
	}
	catch (exception&) {
		for (auto i = objects.begin(); i != objects.end(); ++i) {
			if (!(*i)->target.compare(target))
				return *i;
		}
	}
	return objects[0];
}

GObject* Objects_Manager::findObjectOfID(int id) {
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->ID == id)
			return objects[i];
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

void Objects_Manager::fillObject(GObject* obj, string data) {
	while (!data.empty()) {
		cleanSpaces(data);

		if (identify(data, "target: ")) {
			obj->target = getAndClear(data);
			if (findObject(obj->target) != objects[0])
				std::cout << "Objects with the same target : " << obj->target <<
				". Can result in unexpected behavior" << std::endl;
			continue;
		}
		if (identify(data, "targetnames: ")) {
			istringstream iss(getAndClear(data));
			string word;
			obj->targetnames.clear();
			while (iss >> word) {
				obj->targetnames.push_back(word);
			}
			continue;
		}
		if (identify(data, "texture: ")) {
			obj->textureName = getAndClear(data);
			obj->texture = Textures_Manager::findTexture(obj->textureName);
			continue;
		}
		if (identify(data, "type: ")) {
			obj->type = getAndClear(data);
			continue;
		}
		if (identify(data, "flags: ")) {
			istringstream iss(getAndClear(data));
			string word;
			obj->flags.clear();
			while (iss >> word) {
				obj->flags.push_back(word);
			}
			continue;
		}
		if (identify(data, "crect: ")) {
			continue;
		}
		if (identify(data, "content: ")) {
			obj->content = getAndClear(data);
			continue;
		}
		if (identify(data, "x: ")) {
			obj->x = std::stoi(getAndClear(data));
			continue;
		}
		if (identify(data, "y: ")) {
			obj->y = std::stoi(getAndClear(data));
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
	
	if (findObject(target) != objects[0])
		fillObject(findObject(target), data);
}


//syntax in console : new ent <field1>: <value1>, <field2>: <value2> ...
GObject* Objects_Manager::createObject(string data) {

	GObject* obj = new GObject;

	std::vector<int> sortedID;
	for (int i = 0; i < objects.size(); i++) {
		sortedID.push_back(objects[i]->ID);
	}
	std::sort(sortedID.begin(), sortedID.end());

	for (int i = 0; i < sortedID.size(); i++) {
		if (i == sortedID.size() - 1) {
			obj->ID = sortedID[i] + 1;
			break;
		}
		if (sortedID[i] != sortedID[i + 1] - 1) {
			obj->ID = sortedID[i] + 1;
			break;
		}
	}

	fillObject(obj, data);

	objects.push_back(obj);

	return obj;
}


//beware of new fields
void Objects_Manager::saveObjects() {
	if (tempStream == NULL) return;
	tempStream->close();
	std::ofstream ofs;
	ofs.open(Paths::entData, std::ofstream::out | std::ofstream::trunc);

	for (int i = 0; i < objects.size(); i++) {
		ofs << "ID: " << objects[i]->ID << endl;
		if (!objects[i]->target.empty())
			ofs << "target: " << objects[i]->target << endl;
		if (!objects[i]->targetnames.empty()) {
			ofs << "targetnames:";
			for (int j = 0; j < objects[i]->targetnames.size(); j++)
				ofs << " " << objects[i]->targetnames[j];
			ofs << endl;
		}
		if (!objects[i]->flags.empty()) {
			ofs << "flags:";
			for (int j = 0; j < objects[i]->flags.size(); j++)
				ofs << " " << objects[i]->flags[j];
			ofs << endl;
		}
		ofs << "type: " << objects[i]->type << endl;
		if (!objects[i]->textureName.empty())
			ofs << "texture: " << objects[i]->textureName << endl;
		if (objects[i]->x)
			ofs << "x: " << objects[i]->x << endl;
		if (objects[i]->y)
			ofs << "y: " << objects[i]->y << endl;
		if (!objects[i]->content.empty())
			ofs << "content: " << objects[i]->content << endl;
		ofs << endl << endl;
	}

	ofs << endl << endl << "EOF";
	ofs.close();


}