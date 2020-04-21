#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Textures_Manager.h"
#include "Global_Flags.h"

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


class SpritesHandler {
public:
	SpritesHandler();
	SDL_Texture* currentFrame();
	void setCurrentGroup(std::string group);
	void addGroup(std::string sheet_name, int width_per_sprite, int height_per_sprite, int offsetX, int offsetY, int row_index, int nb_of_frames,
		std::string group_name, int speed = 100, SDL_Color alpha = { ALPHAR, ALPHAG, ALPHAB });
	void setIdle(bool idle);
	void setSingleFrame(std::string textureName);	//For objects with just a single texture

private:
	int currentGroup;

	std::vector<sprite_group> groups;
};