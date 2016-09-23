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
		std::cout << "Image non chargee Error: " << SDL_GetError() << " path: " << path << std::endl;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_Renderer, surface_temp);
	if (newTexture == NULL)
	{
		printf("Texture non chargee Error: %s\n", SDL_GetError());
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

//en cours
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

/*
void Renderer::renderObjects(Personnage* Perso)
{
	if (Perso->rect.x > (camera.x + camera.w) && Perso->rect.x<(Map::x*W))
	{
		camera.x += camera.w;
		camera.px += camera.w / W;
	}
	if ((Perso->rect.x < camera.x) && (Perso->rect.x>0))
	{
		camera.x -= camera.w;
		camera.px -= camera.w / W;
	}
	if (Perso->rect.y >(camera.y + camera.h) && Perso->rect.y<(Map::y*H))
	{
		camera.y += camera.h;
		camera.py += camera.h / W;
	}
	if ((Perso->rect.y < camera.y) && (Perso->rect.y>0))
	{
		camera.y -= camera.h;
		camera.py -= camera.h / H;
	}

	blitty.x = 0;
	blitty.y = 0;

	for (int h = camera.py; h < (camera.py + camera.h / H); h++)
	{
		for (int w = camera.px; w < (camera.px + camera.w / W); w++)
		{
			switch (Map::matrix[h][w])
			{
			case '1':
				SDL_RenderCopy(gRenderer, wall1, NULL, &blitty);
				break;
			case '2':
				SDL_RenderCopy(gRenderer, wallG, NULL, &blitty);
				break;
			case '3':
				SDL_RenderCopy(gRenderer, wallDW, NULL, &blitty);
				break;
			case '4':
				SDL_RenderCopy(gRenderer, wallU, NULL, &blitty);
				break;
			case '5':
				SDL_RenderCopy(gRenderer, wall1, NULL, &blitty);
				break;
			case 'C':
				SDL_RenderCopy(gRenderer, checkpoint, NULL, &blitty);
				break;
			case 'D':
				SDL_RenderCopy(gRenderer, death, NULL, &blitty);
				break;
			}

			blitty.x += W;
		}

		blitty.x = 0;
		blitty.y += H;
	}

	Perso->handleTextures();
	Purse::setPercent();
}*/
