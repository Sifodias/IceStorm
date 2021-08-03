#include "Textures_m.h"
#include <string>
#include <iostream>
#include "Character.h"
#include "Renderer.h"
#include <iostream>
#include "Paths.h"
#include <SDL2/SDL_image.h>
#include "Objects_m.h"
#include "Camera.h"
#include <algorithm>
#include <filesystem>

std::vector<img_struct*> Textures_m::imgList;
bool Textures_m::showInvisibleEnts = SHOWINV;
bool Textures_m::showGrid = false;

void Textures_m::init(std::string str) {
	if (!imgList.size()) {
		switch (Renderer::mode) {
		case SDL:
		imgList.push_back(new img_sdl(NULL, NULL, ""));
		break;

		}
	}
	std::string path = str.empty() ? Paths::texturesPath : str;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!std::filesystem::is_regular_file(entry) || (entry.path().extension() != ".png" && entry.path().extension() != ".tga")) {
			if (std::filesystem::is_directory(entry))
				Textures_m::init(entry.path());

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

int Textures_m::findIndex(std::string name) {
	int i = 0;
	for (img_struct* img : imgList) {
		if (img->name() == name) {
			return i;
		}
		i++;
	}
	return 0;
}

SDL_Texture* Textures_m::findTexture(std::string name) {
	if (!name.size())
		return NULL;
	for (img_struct* img : imgList) {
		if (img->name() == name) {
			return ((img_sdl*)img)->texture;
		}
	}
	return NULL;
}

SDL_Surface* Textures_m::findSurface(std::string name) {
	if (!name.size())
		return NULL;
	for (img_struct* img : imgList) {
		if (img->name() == name) {
			return ((img_sdl*)img)->surface;
		}
	}
	return NULL;
}

void Textures_m::printFrame() {
	SDL_Rect rect_cursor = { 0, 0, GRID_W, GRID_H };

	
	c_rect tati = c_rect((double)Camera::getX(), (double)Camera::getY(),
		Camera::outerRect.w, Camera::outerRect.h);

	std::vector<int> ret = Objects_m::getIntersections(tati);
		 
	for (int id : ret) {
		ob(id).blit();
	}


	// for (Map::mapNode node : toPrint) {
	// 	GObject& obj = Objects_m::findObject(node.id);
	// 	if (!obj.checkFlag("DYNAMIC") && !obj.useMUnit) {
	// 		obj.blit({ node.rect.x - Camera::getX(), node.rect.y - Camera::getY() });
	// 	}
	// }

	// /* Print the dynamic objects */
	// for (auto& [id, obj] : Objects_m::objects) {
	// 	if (!obj.useMUnit || !obj.bounded())
	// 		continue;
	// 	obj.blit();
	// }


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

void Textures_m::quit() {
	for (auto strct : imgList) {
		strct->destroy();
		delete strct;
	}
}