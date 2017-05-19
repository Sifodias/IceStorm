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

std::vector<SDL_Texture*> Textures_Manager::textureList;
std::vector<std::string> Textures_Manager::textureNames;
C_Rect Textures_Manager::camera;

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

void Textures_Manager::TMInit()
{
	textureList = texturesListInit();
	camera.x = 0;
	camera.y = 0;
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
	blitty.x = (int)camera.x;
	blitty.y = (int)camera.y;
	for (int y = 0; y < Map::y; y++, blitty.x = 0, blitty.y += blitty.h) {
		for (int x = 0; x < Map::x; x++, blitty.x += blitty.w) {
			SDL_RenderCopy(Renderer::g_Renderer, 
				Objects_Manager::objects[Map::matrix[y][x]]->texture, NULL, &blitty);
		}
	}
	SDL_RenderCopy(Renderer::g_Renderer, Character::texture, NULL, &(SDL_Rect)Character::hitBox);
}