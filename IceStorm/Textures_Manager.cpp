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
bool showInvisibleEnts = true;


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


//This function works well, but needs to be cleaned
void Textures_Manager::blitStuff()
{
	SDL_Rect blitty;
	blitty.h = GRID_H;
	blitty.w = GRID_W;

	for (int i = 0; i < Map::matrix.size(); i++) {
		blitty.x = -Camera::getX();
		blitty.y = -Camera::getY();

		int maxy = ((Camera::getY() + Camera::outerRect.h) / GRID_H) + 1;
		int maxx = ((Camera::getX() + Camera::outerRect.w) / GRID_W) + 1;
		if (((Camera::getY() + Camera::outerRect.h) / GRID_H) + 1 > Map::matrix[0].size())
			maxy = (int)Map::matrix[0].size();

		int miny = (int)(Camera::getY() / GRID_H);
		int minx = (int)(Camera::getX() / GRID_W);
		if (--miny < 0) miny = 0;
		if (--minx < 0) minx = 0;
		blitty.y += miny * GRID_H;
		blitty.x = -Camera::getX() + minx * GRID_W;
		for (int y = miny; y < maxy; y++, blitty.x = -Camera::getX() + minx * GRID_W, blitty.y += blitty.h) {
			for (int x = minx; x < maxx; x++, blitty.x += blitty.w) {
				if (blitty.x > -GRID_W && blitty.x < Renderer::SCREEN_W &&
					blitty.y > -GRID_H && blitty.y < Renderer::SCREEN_H) {

					SDL_Rect out = Objects_Manager::findObjectOfID(Map::getID(x, y, i))->rect;
					if (out.w > 0 && out.h > 0) {
						//here use the dimensions specified in the object data
						out.x = blitty.x; out.y = blitty.y;
					}
					else {
						out = blitty;
					}
					if (showInvisibleEnts) {
						if (Map::getID(x, y, i)) {
							if (Objects_Manager::findObjectOfID(Map::getID(x, y, i))->texture == NULL) {
								SDL_RenderCopy(Renderer::g_Renderer,
									findTexture("inv.png"), NULL, &blitty);
							}
							else {
									SDL_RenderCopy(Renderer::g_Renderer,
										Objects_Manager::findObjectOfID(Map::getID(x, y, i))->texture, NULL, &out);
							}
						}
					}
					else {
						SDL_RenderCopy(Renderer::g_Renderer,
							Objects_Manager::findObjectOfID(Map::getID(x, y, i))->texture, NULL, &out);
					}
				}
			}
		}
	}

	blitty = (SDL_Rect)Character::movingUnit.hitBox;
	blitty.x -= Camera::getX();
	blitty.y -= Camera::getY();
	blitty.y -= CHAR_H - CHAR_HITBOX_H;
	blitty.h = CHAR_H;
	blitty.w = CHAR_W;
	SDL_RenderCopy(Renderer::g_Renderer, Character::textures.currentFrame(), NULL, &blitty);

	for (int i = 0; i < Objects_Manager::objects.size(); i++) {
		if (!Objects_Manager::objects[i]->x && Objects_Manager::objects[i]->y)
			continue;
		blitty.x = Objects_Manager::objects[i]->movingUnit.hitBox.x;
		blitty.y = Objects_Manager::objects[i]->movingUnit.hitBox.y;
		blitty.w = Objects_Manager::objects[i]->movingUnit.hitBox.w;
		blitty.h = Objects_Manager::objects[i]->movingUnit.hitBox.h;
		SDL_RenderCopy(Renderer::g_Renderer, Objects_Manager::objects[i]->texture, NULL, &blitty);
	}
}