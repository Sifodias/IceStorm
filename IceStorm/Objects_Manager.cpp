#pragma once
#include "Objects_Manager.h"
#include <sstream>
vector<GObject*> Objects_Manager::objects;

void Objects_Manager::Init() {
	ifstream tempStream = loadFile(Paths::entData);
	tempStream.seekg(0);
	std::string buffer;
	std::getline(tempStream, buffer);
	GObject* currentObject;
	while (buffer.compare("EOF")) {
		identify(buffer, "ID: ");
		currentObject = new GObject;
		currentObject->ID = std::stoi(buffer);
		std::getline(tempStream, buffer);
		while (!identify(buffer, "ID: ") && buffer.compare("EOF")) {
			//fill the element
			if (identify(buffer, "target: ")) {
				currentObject->target = buffer;
				if (findObject(buffer) != NULL)
					std::cout << "Objects with the same target : " << buffer <<
					". Can result in unexpected behavior" << std::endl;
				std::getline(tempStream, buffer);
				continue;
			}
			if (identify(buffer, "targetnames: ")) {
				istringstream iss(buffer);
				string word;
				while (iss >> word) {
					currentObject->targetnames.push_back(word);
				}
				std::getline(tempStream, buffer);
				continue;
			}
			if (identify(buffer, "texture: ")) {
				currentObject->textureName = buffer;
				currentObject->texture = Textures_Manager::findTexture(buffer);
				std::getline(tempStream, buffer);
				continue;
			}
			if (identify(buffer, "type: ")) {
				currentObject->type = buffer;
				std::getline(tempStream, buffer);
				continue;
			}
			if (identify(buffer, "flags: ")) {
				istringstream iss(buffer);
				string word;
				while (iss >> word) {
					currentObject->flags.push_back(word);
				}
				std::getline(tempStream, buffer);
				continue;
			}
			if (identify(buffer, "crect: ")) {
				std::getline(tempStream, buffer);
				continue;
			}
			if (identify(buffer, "content: ")) {
				currentObject->content = buffer;
				std::getline(tempStream, buffer);
				continue;
			}
			else std::getline(tempStream, buffer);
		}
		objects.push_back(currentObject);
	}
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
