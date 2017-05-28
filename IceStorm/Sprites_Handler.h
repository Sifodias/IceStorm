#pragma once
#include <SDL.h>
#include <vector>
#include <sstream>
#include "Textures_Manager.h"
using namespace std;

class SpritesHandler {
public:
	void Init(string str, string names, int ispeed = 5) {
		speed = ispeed;
		timerA = timerB = SDL_GetTicks();
		istringstream iss(str);
		string word;
		int i = 0;
		while (iss >> word) {
			if (!word.compare("|")) {
				i++;
				continue;
			}
			if (i == textures.size()) {
				vector<SDL_Texture*> tempy;
				textures.push_back(tempy);
			}
			textures[i].push_back(Textures_Manager::findTexture(word));
		}
		istringstream isb(names);
		while (isb >> word) {
			groups.push_back(word);
		}
	}
	SDL_Texture* currentFrame() {
		//
		//
		return textures[0][0];
	}
	Uint32 timerA;
	Uint32 timerB;
	vector<vector<SDL_Texture*>> textures;
	vector<string> groups;
	int speed;
};
