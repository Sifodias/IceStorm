#pragma once
#include "Generic_Object.h"
#include <vector>
#include <ostream>
#include "Paths.h"
#include "Engine_Manager.h"
#include "Textures_Manager.h"
using namespace std;

class Objects_Manager {
public:
	Objects_Manager() {
		init_OM();
	}

	void init_OM() {
		ifstream tempStream = loadFile(Paths::objectsDatabasePath);
		tempStream.seekg(0);
		std::string buffer;
		std::getline(tempStream, buffer);
		GObject* currentObject;
		while (buffer.compare("EOF")) {
			if (buffer.find("ID: ") != std::string::npos) {
				currentObject = new GObject;
				objects.push_back(currentObject);
				buffer.erase(buffer.begin(), buffer.begin() + 5);
				//std::cout << buffer << std::endl;
				currentObject->ID = std::stoi(buffer);
				//std::cout << currentObject->ID << std::endl;
				std::getline(tempStream, buffer);
				while (buffer.find("ID: ") == std::string::npos
					&& buffer.find("EOF") == std::string::npos) {
					//fill the element
					if (buffer.find("target: ") != std::string::npos) {
						continue;
					}
					if (buffer.find("targetnames: ") != std::string::npos) {
						continue;
					}
					if (buffer.find("texture ID: ") != std::string::npos) {
						buffer.erase(buffer.begin(), buffer.begin() + 12);
						currentObject->texture = Textures_Manager::textureList[std::stoi(buffer)];
						continue;
					}
					if (buffer.find("type: ") != std::string::npos) {
						continue;
					}
					if (buffer.find("flags: ") != std::string::npos) {
						continue;
					}
					if (buffer.find("crect: ") != std::string::npos) {
						continue;
					}
					if (buffer.find("content: ") != std::string::npos) {
						continue;
					}
					std::getline(tempStream, buffer);
				}
			}
			std::getline(tempStream, buffer);
		}
	}

	GObject* find(string target) {
		for (auto i = objects.begin(); i != objects.end(); ++i) {
			if (!(*i)->target.compare(target))
				return *i;
		}
		return NULL;
	}

	static vector<GObject*> objects;
};

/*
Data structure of the object database :
ID: <int ID>
target: <string target>
targetnames: <strings targetnames>
texture ID: <int texture id>
type: <string type>
flags: <strings flags>
crect: <h, w>
content: <string content>
*/