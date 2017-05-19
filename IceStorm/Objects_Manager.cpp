#pragma once
#include "Objects_Manager.h"

vector<GObject*> Objects_Manager::objects;

void Objects_Manager::init_OM() {
	ifstream tempStream = loadFile(Paths::entData);
	tempStream.seekg(0);
	std::string buffer;
	std::getline(tempStream, buffer);
	GObject* currentObject;
	while (buffer.compare("EOF")) {
		if (identify(buffer, "ID: ")) {
			currentObject = new GObject;
			objects.push_back(currentObject);
			buffer.erase(buffer.begin(), buffer.begin() + 4);
			currentObject->ID = std::stoi(buffer);
			std::getline(tempStream, buffer);
			while (!identify(buffer, "ID: ") && !identify(buffer, "EOF")) {
				//fill the element
				if (identify(buffer, "target: ")) {
					buffer.erase(buffer.begin(), buffer.begin() + 8);
					currentObject->target = buffer;
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "targetnames: ")) {
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "texture: ")) {
					buffer.erase(buffer.begin(), buffer.begin() + 9);
					currentObject->texture = Textures_Manager::findTexture(buffer);
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "type: ")) {
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "flags: ")) {
					buffer.erase(buffer.begin(), buffer.begin() + 7);
					if (!buffer.compare("SOLID")) currentObject->solid = 1;
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "crect: ")) {
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "content: ")) {
					std::getline(tempStream, buffer);
					continue;
				}
				else std::getline(tempStream, buffer);
			}
		}
	}
}

bool Objects_Manager::identify(string target, string wanted)
{
	if (target.size() < wanted.size()) return false;
	target.erase(wanted.size(), target.size());
	return !target.compare(wanted);
}
