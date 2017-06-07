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

std::vector<SDL_Texture*> Textures_Manager::texturesListInit()
{
	std::ifstream tempStream = loadFile(Paths::texturesListPath);
	std::vector<SDL_Texture*> tempVec;

	if (!tempStream) {
		printf("ERROR texturelist not loaded\n");
		return tempVec;
	}
	std::string catcher;
	SDL_Texture* textTemp;

	tempStream.clear();
	tempStream.seekg(0);
	while (!tempStream.eof()) {
		getline(tempStream, catcher);
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
	blitty.x = -(int)Camera::getX();
	//blitty.x = Character::hitBox.x-120; blitty.y = Character::hitBox.y-160;
	blitty.y = -(int)Camera::Camera::getY();
	for (int y = 0; y < Map::y; y++, blitty.x = -(int)Camera::getX(),
		blitty.y += blitty.h) {
		for (int x = 0; x < Map::x; x++, blitty.x += blitty.w) {
			SDL_RenderCopy(Renderer::g_Renderer,
				Objects_Manager::objects[Map::matrix[y][x]]->texture, NULL, &blitty);
		}
	}
	blitty = (SDL_Rect)Character::hitBox;
	std::cout << Camera::getX() << " " << Camera::getY() << " " <<
		Character::hitBox.x << " "<< Character::hitBox.y <<::endl;
	blitty.x -= Camera::getX();
	blitty.y -= Camera::getY();
	blitty.y -= CHAR_H - CHAR_HITBOX_H;
	blitty.h = CHAR_H;
	SDL_RenderCopy(Renderer::g_Renderer, Character::textures.currentFrame(), NULL, &blitty);
}