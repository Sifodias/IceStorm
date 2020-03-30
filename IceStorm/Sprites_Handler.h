#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Textures_Manager.h"


//need to be able to handle spritesheets
class SpritesHandler {
public:
	SpritesHandler(std::string str = std::string(), std::string names = std::string(), int ispeed = 100);
	SDL_Texture* currentFrame();
	void setCurrentGroup(std::string group);

private:
	Uint32 timerA;
	Uint32 timerB;
	std::vector<SDL_Texture*>* currentGroup;
	std::vector<std::vector<SDL_Texture*>> textures;
	std::vector<std::string> groups;
	int speed;
};