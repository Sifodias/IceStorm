#include "Textures_Manager.h"
#include <string>
#include <iostream>
#include "Character.h"
#include "Renderer.h"
#include <iostream>
#include "Paths.h"
#include "Map.h"
#include <SDL_image.h>
#include "Engine_Manager.h"
#include "Objects_Manager.h"
#include "Camera.h"
#include <algorithm>


class img_struct {
public:
	img_struct(SDL_Texture* t, SDL_Surface* s, std::string n) : texture(t), surface(s), name(n) {}
	//~img_struct() {
	//	SDL_DestroyTexture(texture);
	//	SDL_FreeSurface(surface);
	//}

	SDL_Texture* texture;
	SDL_Surface* surface;
	std::string name;
};

std::vector<img_struct> imgList;
bool Textures_Manager::showInvisibleEnts = true;


void Textures_Manager::init()
{
	std::ifstream* tempStream = loadFile(Paths::texturesListPath);
	std::vector<SDL_Texture*> tempVec;

	if (!tempStream) {
		printf("ERROR texturelist not loaded\n");
		return;
	}

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
	//SDL_Rect blitty;
	//blitty.h = GRID_H;
	//blitty.w = GRID_W;
	//for (int i = 0; i < Map::matrix.size(); i++) {
	//	blitty.x = -Camera::getX();
	//	blitty.y = -Camera::getY();
	//	int maxy = ((Camera::getY() + Camera::outerRect.h) / GRID_H) + 1;
	//	int maxx = ((Camera::getX() + Camera::outerRect.w) / GRID_W) + 1;
	//	if (((Camera::getY() + Camera::outerRect.h) / GRID_H) + 1 > Map::matrix[0].size())
	//		maxy = (int)Map::matrix[0].size();
	//	int miny = (int)(Camera::getY() / GRID_H);
	//	int minx = (int)(Camera::getX() / GRID_W);
	//	if (--miny < 0) miny = 0;
	//	if (--minx < 0) minx = 0;
	//	blitty.y += miny * GRID_H;
	//	blitty.x = -Camera::getX() + minx * GRID_W;
	//	for (int y = miny; y < maxy; y++, blitty.x = -Camera::getX() + minx * GRID_W, blitty.y += blitty.h) {
	//		for (int x = minx; x < maxx; x++, blitty.x += blitty.w) {
	//			if (blitty.x > -GRID_W && blitty.x < Renderer::SCREEN_W &&
	//				blitty.y > -GRID_H && blitty.y < Renderer::SCREEN_H) {
	//				SDL_Rect out = Objects_Manager::findObject(Map::getID(x, y, i)).rect;
	//				if (out.w > 0 && out.h > 0) {
	//					//here use the dimensions specified in the object data
	//					out.x = blitty.x; out.y = blitty.y;
	//				}
	//				else {
	//					out = blitty;
	//				}
	//				if (showInvisibleEnts) {
	//					if (Map::getID(x, y, i)) {
	//						if (Objects_Manager::findObject(Map::getID(x, y, i)).texture == NULL) {
	//							SDL_RenderCopy(Renderer::g_Renderer,
	//								findTexture("inv.png"), NULL, &blitty);
	//						}
	//						else {
	//								SDL_RenderCopy(Renderer::g_Renderer,
	//									Objects_Manager::findObject(Map::getID(x, y, i)).texture, NULL, &out);
	//						}
	//					}
	//				}
	//				else {
	//					SDL_RenderCopy(Renderer::g_Renderer,
	//						Objects_Manager::findObject(Map::getID(x, y, i)).texture, NULL, &out);
	//				}
	//			}
	//		}
	//	}
	//}

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
				if (!Map::getID(x, y, i))
					continue;

				GObject& currentObj = Objects_Manager::findObject(Map::getID(x, y, i));
				SDL_Rect out = currentObj.rect;
				if (out.w > 0 && out.h > 0) {
					/* Here use the dimensions specified in the object data */
					out.x = rect_cursor.x; out.y = rect_cursor.y;
				}
				else
					out = rect_cursor;

				SDL_Texture* to_print = currentObj.texture;

				/* If the object has no texture and we must display every ent, give generic texture */
				to_print = to_print != NULL ? to_print : (showInvisibleEnts ? findTexture("inv.png") : NULL);

				SDL_RenderCopy(Renderer::g_Renderer, to_print, NULL, &out);
			}
		}

		/* Update the index of the current plan */
		i++;
	}

	/* Print the character */
	rect_cursor = (SDL_Rect)Character::movingUnit.hitBox;
	rect_cursor.x -= Camera::getX();
	rect_cursor.y -= Camera::getY();
	rect_cursor.y -= CHAR_H - CHAR_HITBOX_H;
	rect_cursor.h = CHAR_H;
	rect_cursor.w = CHAR_W;
	SDL_RenderCopy(Renderer::g_Renderer, Character::textures.currentFrame(), NULL, &rect_cursor);

	/* Print the dynamic objects */
	for (GObject& obj : Objects_Manager::objects) {
		if (!obj.x && obj.y)
			continue;
		rect_cursor = obj.movingUnit.hitBox;
		SDL_RenderCopy(Renderer::g_Renderer, obj.texture, NULL, &rect_cursor);
	}
}