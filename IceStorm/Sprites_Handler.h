#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Textures_Manager.h"

class sprite_group {
public:
	sprite_group(std::vector<SDL_Texture*> t, int s, std::string n) : speed(s), name(n), idle(true) {
		for (int i = 0; i < t.size(); i++) {
			textures.push_back(t[i]);
		}
	}
	//~sprite_group() {
	//	for (SDL_Texture* texture : textures) {
	//		SDL_DestroyTexture(texture);
	//	}
	//}

	std::vector<SDL_Texture*> textures;
	int speed;
	std::string name;
	int idle;
};


//need to be able to handle spritesheets
class SpritesHandler {
public:
	SpritesHandler();
	SDL_Texture* currentFrame();
	void setCurrentGroup(std::string group);
	void addGroup(std::string sheet_name, int width_per_sprite, int height_per_sprite, int offsetX, int offsetY, int row_index, int nb_of_frames,
		std::string group_name, int speed = 100, SDL_Color alpha = { 195, 134, 255 });
	void setIdle(bool idle);

private:
	int currentGroup;

	std::vector<sprite_group> groups;
};