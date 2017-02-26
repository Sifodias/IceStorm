#include "Textures_Manager.h"
#include <string>
#include <iostream>
#include "Character.h"
std::vector<SDL_Texture*> Textures_Manager::textureList;
C_Rect Textures_Manager::camera;

std::vector<SDL_Texture*> Textures_Manager::texturesListInit()
{
	std::ifstream tempStream = loadFromTxt(Paths::texturesListPath);
	std::vector<SDL_Texture*> tempVec;

	if (!tempStream)
	{
		printf("ERROR texturelist not loaded\n");
		return tempVec;
	}
	std::string catcher;
	SDL_Texture* textTemp;

	tempStream.clear();
	tempStream.seekg(0);
	while (!tempStream.eof()) {
		getline(tempStream, catcher);
		catcher.insert(0, "./Textures/");
		std::cout << catcher << endl;
		textTemp = loadTexture(catcher);
		tempVec.push_back(textTemp);
	}

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

void Textures_Manager::C_RenderCopy(SDL_Texture * text, C_Rect rect)
{
	SDL_Rect tempRect;
	tempRect.x = (int)rect.x;
	tempRect.y = (int)rect.y;
	tempRect.h = (int)rect.h;
	tempRect.w = (int)rect.w;
	SDL_RenderCopy(Renderer::g_Renderer, text, NULL, &tempRect);
}

void Textures_Manager::blitStuff()
{
	SDL_Rect blitty;
	blitty.h = GRID_H;
	blitty.w = GRID_W;
	blitty.x = (int) camera.x;
	blitty.y = (int) camera.y;
	for (int y = 0; y < Map::y; y++, blitty.x = 0, blitty.y += blitty.h)
	{
		for (int x = 0; x < Map::x; x++, blitty.x += blitty.w)
		{
			SDL_RenderCopy(Renderer::g_Renderer, textureList[Map::matrix[y][x]], NULL, &blitty);
		}
	}
	C_RenderCopy(Character::texture, Character::hitBox);
}