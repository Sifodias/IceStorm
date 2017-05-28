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
		if (identify(buffer, "ID: ")) {
			currentObject = new GObject;
			buffer.erase(buffer.begin(), buffer.begin() + 4);
			currentObject->ID = std::stoi(buffer);
			std::getline(tempStream, buffer);
			while (!identify(buffer, "ID: ") && !identify(buffer, "EOF")) {
				//fill the element
				if (identify(buffer, "target: ")) {
					buffer.erase(buffer.begin(), buffer.begin() + 8);
					currentObject->target = buffer;
					if (findObject(buffer) != NULL)
						std::cout << "Objects with the same target : " << buffer <<
						". Can result in unexpected behavior" << std::endl;
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "targetnames: ")) {
					buffer.erase(buffer.begin(), buffer.begin() + 13);
					istringstream iss(buffer);
					string word;
					while (iss >> word) {
						currentObject->targetnames.push_back(word);
					}
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
					buffer.erase(buffer.begin(), buffer.begin() + 6);
					currentObject->type = buffer;
					std::getline(tempStream, buffer);
					continue;
				}
				if (identify(buffer, "flags: ")) {
					buffer.erase(buffer.begin(), buffer.begin() + 7);
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
					buffer.erase(buffer.begin(), buffer.begin() + 9);
					currentObject->content = buffer;
					std::getline(tempStream, buffer);
					continue;
				}
				else std::getline(tempStream, buffer);
			}
			objects.push_back(currentObject);
		}
	}
}

bool Objects_Manager::identify(string target, string wanted)
{
	if (target.size() < wanted.size()) return false;
	target.erase(wanted.size(), target.size());
	return !target.compare(wanted);
}
