#pragma once
#include "Objects_Manager.h"
#include <sstream>
#include <ostream>
#include <iostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"

vector<GObject*> Objects_Manager::objects;
ifstream* Objects_Manager::tempStream = NULL;

void Objects_Manager::Init() {
	tempStream = loadFile(Paths::entData);
	if (tempStream == NULL) {
		printf("Couldn't load data objects file");
		return;
	}
	createObject(1);
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

void Objects_Manager::createObject(int mode, string data)
{
	std::string buffer;
	if (mode == 1) {
		tempStream->seekg(0);
		std::getline(*tempStream, buffer);
	}

	GObject* currentObject;
	while (buffer.compare("EOF")) {
		identify(buffer, "ID: ");
		currentObject = new GObject;
		currentObject->ID = std::stoi(buffer);
		std::getline(*tempStream, buffer);
		while (!identify(buffer, "ID: ") && buffer.compare("EOF")) {
			//fill the element
			if (identify(buffer, "target: ")) {
				currentObject->target = buffer;
				if (findObject(buffer) != NULL)
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
			else std::getline(*tempStream, buffer);
		}
		objects.push_back(currentObject);
		if (mode == 0)
			break;
	}
}

GObject* Objects_Manager::findObject(string target) {
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		if (!(*i)->target.compare(target))
			return *i;
	}
	return NULL;
}