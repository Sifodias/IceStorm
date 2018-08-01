#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Textures_Manager.h"


//need to be able to handle spritesheets
class SpritesHandler {
public:
	void init(std::string str, std::string names, int ispeed = 100) {
		currentGroup = NULL;
		speed =ispeed;
		timerA = timerB = SDL_GetTicks();
		std::istringstream iss(str);
		std::string word;
		int i = 0;
		while (iss >> word) {
			if (!word.compare("|")) {
				i++;
				continue;
			}
			if (i == textures.size()) {
				std::vector<SDL_Texture*> tempy;
				textures.push_back(tempy);
			}
			textures[i].push_back(Textures_Manager::findTexture(word));
		}
		std::istringstream isb(names);
		while (isb >> word) {
			groups.push_back(word);
		}
	}
	SDL_Texture* currentFrame() {
		if (currentGroup == NULL)
			return textures[0][0];

		timerB = SDL_GetTicks();
		return (*currentGroup)[((timerB - timerA) / speed) % (currentGroup->size())];
	}
	void setCurrentGroup(std::string group) {
		int y = 0;
		for (auto i = groups.begin(); i != groups.end(); ++i, y++) {
			if (!i->compare(group)) {
				currentGroup = &textures[y];
				break;
			}
		}
	}
	Uint32 timerA;
	Uint32 timerB;
	std::vector<SDL_Texture*>* currentGroup;
	std::vector<std::vector<SDL_Texture*>> textures;
	std::vector<std::string> groups;
	int speed;
};