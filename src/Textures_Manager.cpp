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


std::vector<img_struct> Textures_Manager::imgList;
bool Textures_Manager::showInvisibleEnts = SHOWINV;
bool Textures_Manager::showGrid = false;

void Textures_Manager::init()
{
	std::ifstream* tempStream = loadFile(Paths::texturesListPath);
	std::vector<SDL_Texture*> tempVec;

	if (!tempStream) {
		printf("ERROR texturelist not loaded\n");
		return;
	}
	imgList.push_back({ NULL, NULL, "" });
	std::string catcher;

	tempStream->clear();
	tempStream->seekg(0);
	while (!tempStream->eof()) {
		getline(*tempStream, catcher);

		SDL_Surface* new_surface = IMG_Load(("./Textures/" + catcher).c_str());
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
		imgList.push_back(img_struct(newTexture, new_surface, catcher));
	}
}


int Textures_Manager::findIndex(std::string name) {
	int i = 0;
	for (img_struct& img : imgList) {
		if (img.name == name) {
			return i;
		}
		i++;
	}
	return 0;
}
SDL_Texture* Textures_Manager::findTexture(std::string name)
{
	if (!name.size()) return NULL;
	for (img_struct& img : imgList) {
		if (img.name == name) {
			return img.texture;
		}
	}
	return NULL;
}

SDL_Surface* Textures_Manager::findSurface(std::string name)
{
	if (!name.size()) return NULL;
	for (img_struct& img : imgList) {
		if (img.name == name) {
			return img.surface;
		}
	}
	return NULL;
}


void Textures_Manager::printFrame()
{
	SDL_Rect rect_cursor = { 0, 0, GRID_W, GRID_H };

	/* For each map plan */
	int i = 0;
	for (std::vector<std::vector<int>>& plan : Map::matrix) {
		rect_cursor.x = -Camera::getX();
		rect_cursor.y = -Camera::getY();

		/* We won't print further than those matrix indexes */
		/* Note that we trim out only the maximal indexes */
		int y_limit_index = std::min(Camera::getY() + Camera::outerRect.h + GRID_H, (int)(plan.size()) * GRID_H) / GRID_H;
		int x_limit_index;

		for (int y = 0; y < y_limit_index; y++,
			rect_cursor.x = -Camera::getX(), rect_cursor.y += rect_cursor.h) {

			/* Update the maximal index x for the current index y */
			x_limit_index = std::min((Camera::getX() + Camera::outerRect.w + GRID_W) / GRID_W, (int)plan[y].size());

			for (int x = 0; x < x_limit_index; x++, rect_cursor.x += rect_cursor.w) {
				if (!Map::getID(x, y, i)) {
					if (showGrid) {
						SDL_Rect out = { rect_cursor.x, rect_cursor.y, GRID_W, GRID_H };
						SDL_RenderCopy(Renderer::g_Renderer, Textures_Manager::findTexture("grid.png"), NULL, &out);
					}
					continue;
				}

				GObject& currentObj = Objects_Manager::findObject(Map::getID(x, y, i));
				SDL_Rect out = rect_cursor;

				SDL_Surface* tempSrf = imgList[currentObj.imgIndex].surface;

				if (tempSrf != NULL) {
					out.w = tempSrf->w;
					out.h = tempSrf->h;
				}

				SDL_Texture* to_print = imgList[currentObj.imgIndex].texture;

				/* If the object has no texture and we must display every ent, give generic texture */
				to_print = to_print != NULL ? to_print : (showInvisibleEnts ? findTexture("inv.png") : NULL);
				if (!showInvisibleEnts && currentObj.checkFlag("INV")) {
					to_print = NULL;
				}

				SDL_RenderCopy(Renderer::g_Renderer, to_print, NULL, &out);

				if (showGrid) {
					out.w = GRID_W; out.h = GRID_H;
					SDL_RenderCopy(Renderer::g_Renderer, Textures_Manager::findTexture("grid.png"), NULL, &out);
				}
			}
		}

		/* Update the index of the current plan */
		i++;
	}


	/* Print the dynamic objects */
	for (GObject& obj : Objects_Manager::objects) {
		if (!obj.checkFlag("DYNAMIC"))
			continue;
		rect_cursor = obj.movingUnit.hitBox.sdl();
		rect_cursor.x -= Camera::getX();
		rect_cursor.y -= Camera::getY();

		SDL_RenderCopy(Renderer::g_Renderer, obj.textures.currentFrame(), NULL, &rect_cursor);
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