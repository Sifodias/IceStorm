#include "Textures_Manager.h"
#include <string>
#include <iostream>
#include "Character.h"
#include "Renderer.h"
#include <iostream>
#include "Paths.h"
#include "Map.h"
#include <SDL2/SDL_image.h>
#include "Engine_Manager.h"
#include "Objects_Manager.h"
#include "Camera.h"
#include <algorithm>
#include <filesystem>

std::vector<img_struct*> Textures_Manager::imgList;
bool Textures_Manager::showInvisibleEnts = SHOWINV;
bool Textures_Manager::showGrid = false;

void Textures_Manager::init(std::string str) {
	if (!imgList.size()) {
		switch (Renderer::mode) {
		case SDL:
		imgList.push_back(new img_sdl(NULL, NULL, ""));
		break;

		}
	}
	std::string path = str.empty() ? Paths::texturesPath : str;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!std::filesystem::is_regular_file(entry) || entry.path().extension() != ".png") {
			if (std::filesystem::is_directory(entry))
				Textures_Manager::init(entry.path());

			continue;
		}

		switch (Renderer::mode) {
		case SDL:

		SDL_Surface* new_surface = IMG_Load(entry.path().c_str());
		if (new_surface == NULL) {
			std::cout << "Image not loaded ! Error: " << SDL_GetError() << std::endl;
			continue;
		}
		SDL_SetColorKey(new_surface, SDL_TRUE, SDL_MapRGB(new_surface->format, ALPHAR, ALPHAG, ALPHAB));
		SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_Renderer, new_surface);
		if (newTexture == NULL) {
			std::cout << "Texture not loaded ! Error: " << SDL_GetError() << std::endl;
			continue;
		}
		imgList.push_back(new img_sdl(newTexture, new_surface, entry.path().filename()));
		
		break;
		}
	}
}

int Textures_Manager::findIndex(std::string name) {
	int i = 0;
	for (img_struct* img : imgList) {
		if (img->name() == name) {
			return i;
		}
		i++;
	}
	return 0;
}

SDL_Texture* Textures_Manager::findTexture(std::string name) {
	if (!name.size())
		return NULL;
	for (img_struct* img : imgList) {
		if (img->name() == name) {
			return ((img_sdl*)img)->texture;
		}
	}
	return NULL;
}

SDL_Surface* Textures_Manager::findSurface(std::string name) {
	if (!name.size())
		return NULL;
	for (img_struct* img : imgList) {
		if (img->name() == name) {
			return ((img_sdl*)img)->surface;
		}
	}
	return NULL;
}

void Textures_Manager::printFrame() {
	SDL_Rect rect_cursor = { 0, 0, GRID_W, GRID_H };

	/* For each map plan */
	int i = 0;
	for (std::vector<std::vector<int>>& plan : Map::matrix) {
		auto [cam_x, cam_y] = Camera::getCoord();
		rect_cursor.x = -cam_x;
		rect_cursor.y = -cam_y;

		/* We won't print further than those matrix indexes */
		/* Note that we trim out only the maximal indexes */
		//int y_limit_index = std::min(Camera::getY() + Camera::outerRect.h + GRID_H, (int)(plan.size()) * GRID_H) / GRID_H;
		int y_limit_index = plan.size();
		int x_limit_index = 0;
		// for (auto vec : plan)
		// 	x_limit_index = std::max(x_limit_index, (int)vec.size());

		for (int y = 0; y < y_limit_index; y++,
			rect_cursor.x = -cam_x, rect_cursor.y += rect_cursor.h) {

			/* Update the maximal index x for the current index y */
			x_limit_index = std::min((cam_x + Camera::outerRect.w + GRID_W) / GRID_W, (int)plan[y].size());


			for (int x = 0; x < x_limit_index; x++, rect_cursor.x += rect_cursor.w) {
				if (!Map::getID(x, y, i)) {
					if (showGrid) {
						SDL_Rect out = { rect_cursor.x, rect_cursor.y, GRID_W, GRID_H };
						SDL_RenderCopy(Renderer::g_Renderer, Textures_Manager::findTexture("grid.png"), NULL, &out);
					}
					continue;
				}

				GObject& currentObj = Objects_Manager::findObject(Map::getID(x, y, i));
				if (!currentObj.checkFlag("DYNAMIC")) {
					currentObj.blit({ rect_cursor.x, rect_cursor.y });
				}

				if (showGrid) {
					SDL_Rect out = rect_cursor;
					out.w = GRID_W;
					out.h = GRID_H;
					SDL_RenderCopy(Renderer::g_Renderer, Textures_Manager::findTexture("grid.png"), NULL, &out);
				}
			}
		}

		/* Update the index of the current plan */
		i++;
	}

	/* Print the dynamic objects */
	for (GObject& obj : Objects_Manager::objects) {
		if (!obj.checkFlag("DYNAMIC") || obj.bounded())
			continue;
		obj.blit();
	}

	/* Print the character */
	rect_cursor = Character::movingUnit.hitBox.sdl();
	rect_cursor.x -= Camera::getX();
	rect_cursor.y -= Camera::getY();
	if (Character::useMainOffsets) {
		rect_cursor.y -= CHAR_H - CHAR_HITBOX_H;
		rect_cursor.w = CHAR_W;
		rect_cursor.h = CHAR_H;
	}
	else {
		rect_cursor.w = Character::movingUnit.hitBox.w;
		rect_cursor.h = Character::movingUnit.hitBox.h;
	}
	SDL_RenderCopy(Renderer::g_Renderer, Character::textures.currentFrame(), NULL, &rect_cursor);
}

void Textures_Manager::quit() {
	for (auto strct : imgList) {
		strct->destroy();
		delete strct;
	}
}