#include "Textures_Manager.h"
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
		std::cout << catcher;
		textTemp = loadTexture(catcher);
		tempVec.push_back(textTemp);
	}

	return tempVec;
}

SDL_Texture* Textures_Manager::loadTexture(std::string path)
{
	SDL_Surface* surface_temp = IMG_Load(path.c_str());
	if (surface_temp == NULL)
	{
		std::cout << "Image not loaded Error: " << SDL_GetError() << " path: " << path << std::endl;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_Renderer, surface_temp);
	if (newTexture == NULL)
	{
		printf("Texture not loaded Error: %s\n", SDL_GetError());
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

//In progress, need to manage camera and stuff
void Textures_Manager::blitStuff()
{
	SDL_Rect blitty;
	blitty.h = GRID_H;
	blitty.w = GRID_W;
	blitty.x = 0;
	blitty.y = 0;
	for (int y = 0; y < Map::y; y++, blitty.x = 0, blitty.y += blitty.h)
	{
		for (int x = 0; x < Map::x; x++, blitty.x += blitty.w)
		{
			SDL_RenderCopy(Renderer::g_Renderer, textureList[Map::matrix[y][x]], NULL, &blitty);
		}
	}
}