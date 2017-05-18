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
		if (buffer.find("ID: ") != std::string::npos) {
			currentObject = new GObject;
			objects.push_back(currentObject);
			buffer.erase(buffer.begin(), buffer.begin() + 4);
			currentObject->ID = std::stoi(buffer);
			std::getline(tempStream, buffer);
			while (buffer.find("ID: ") == std::string::npos
				&& buffer.find("EOF") == std::string::npos) {
				//fill the element
				if (buffer.find("target: ") != std::string::npos) {
					buffer.erase(buffer.begin(), buffer.begin() + 8);
					currentObject->target = buffer;
					std::getline(tempStream, buffer);
					continue;
				}
				if (buffer.find("targetnames: ") != std::string::npos) {
					std::getline(tempStream, buffer);
					continue;
				}
				if (buffer.find("texture ID: ") != std::string::npos) {
					buffer.erase(buffer.begin(), buffer.begin() + 12);
					currentObject->texture = Textures_Manager::textureList[std::stoi(buffer)];
					std::getline(tempStream, buffer);
					continue;
				}
				if (buffer.find("type: ") != std::string::npos) {
					std::getline(tempStream, buffer);
					continue;
				}
				if (buffer.find("flags: ") != std::string::npos) {
					buffer.erase(buffer.begin(), buffer.begin() + 7);
					if (!buffer.compare("SOLID")) currentObject->solid = 1;
					std::getline(tempStream, buffer);
					continue;
				}
				if (buffer.find("crect: ") != std::string::npos) {
					std::getline(tempStream, buffer);
					continue;
				}
				if (buffer.find("content: ") != std::string::npos) {
					std::getline(tempStream, buffer);
					continue;
				}
				std::getline(tempStream, buffer);
			}
		}
		std::getline(tempStream, buffer);
	}
}