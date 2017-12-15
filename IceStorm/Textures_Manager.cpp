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

std::vector<SDL_Texture*> Textures_Manager::textureList;
std::vector<std::string> Textures_Manager::textureNames;
SDL_Texture* Textures_Manager::levelScreenshot = NULL;

std::vector<SDL_Texture*> Textures_Manager::texturesListInit()
{
	std::ifstream* tempStream = loadFile(Paths::texturesListPath);
	std::vector<SDL_Texture*> tempVec;

	if (!tempStream) {
		printf("ERROR texturelist not loaded\n");
		return tempVec;
	}
	std::string catcher;
	SDL_Texture* textTemp;

	tempStream->clear();
	tempStream->seekg(0);
	while (!tempStream->eof()) {
		getline(*tempStream, catcher);
		textureNames.push_back(catcher);
		catcher.insert(0, "./Textures/");
		std::cout << catcher << endl;
		textTemp = loadTexture(catcher);
		tempVec.push_back(textTemp);
	}
	if (tempVec.size() != textureNames.size())
		std::cout << "Size of texture vectors not matching wtf" << std::endl;
	return tempVec;
}

SDL_Texture* Textures_Manager::loadTexture(std::string path)
{
	SDL_Surface* surface_temp = IMG_Load(path.c_str());
	if (surface_temp == NULL) {
		std::cout << "Image not loaded ! Error: " << SDL_GetError() << std::endl;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_Renderer, surface_temp);
	if (newTexture == NULL) {
		std::cout << "Texture not loaded ! Error: " << SDL_GetError() << std::endl;
	}
	SDL_FreeSurface(surface_temp);
	return newTexture;
}

void Textures_Manager::Init()
{
	textureList = texturesListInit();
}

SDL_Texture * Textures_Manager::findTexture(std::string name)
{
	if (!name.size()) return NULL;
	int y = 0;
	for (auto i = textureNames.begin(); i != textureNames.end(); ++i, y++) {
		if (!i->compare(name)) return textureList[y];
	}
	return NULL;
}

void Textures_Manager::blitStuff()
{
	SDL_Rect blitty;
	blitty.h = GRID_H;
	blitty.w = GRID_W;
	blitty.x = -Camera::getX();

	blitty.y = -Camera::getY();
	
	int maxy = ((Camera::getY() + Camera::outerRect.h)/ GRID_H) + 1;
	int maxx = ((Camera::getX() + Camera::outerRect.w) / GRID_W) + 1;
	if (((Camera::getY() + Camera::outerRect.h) / GRID_H) + 1 > Map::matrix.size())
		maxy = (int)Map::matrix.size();
	if (((Camera::getX() + Camera::outerRect.w) / GRID_W) + 1 > Map::matrix[0].size())
		maxx = (int)Map::matrix[0].size();
	int miny = (int)(Camera::getY() / GRID_H);
	int minx = (int)(Camera::getX() / GRID_W);
	if (--miny < 0) miny = 0;
	if (--minx < 0) minx = 0;
	blitty.y += miny*GRID_H;
	for (int y = miny; y < maxy; y++, blitty.x = -Camera::getX()+minx*GRID_W, blitty.y += blitty.h) {
		for (int x = minx; x < maxx; x++, blitty.x += blitty.w) {
			if (blitty.x > -GRID_W && blitty.x < Renderer::SCREEN_W &&
				blitty.y > -GRID_H && blitty.y < Renderer::SCREEN_H)
				SDL_RenderCopy(Renderer::g_Renderer,
					Objects_Manager::objects[Map::getID(x,y)]->texture, NULL, &blitty);
		}
	}

	blitty = (SDL_Rect)Character::movingUnit.hitBox;

	blitty.x -= Camera::getX();
	blitty.y -= Camera::getY();
	blitty.y -= CHAR_H - CHAR_HITBOX_H;
	blitty.h = CHAR_H;
	blitty.w = CHAR_W;
	SDL_RenderCopy(Renderer::g_Renderer, Character::textures.currentFrame(), NULL, &blitty);
}