#include "Sprites_Handler.h"
#include "Renderer.h"

SDL_Texture* crop_surface(SDL_Surface* sprite_sheet, int x, int y, int width, int height)
{
	SDL_Surface* surface = SDL_CreateRGBSurface(sprite_sheet->flags, width, height, sprite_sheet->format->BitsPerPixel, sprite_sheet->format->Rmask, sprite_sheet->format->Gmask, sprite_sheet->format->Bmask, sprite_sheet->format->Amask);
	SDL_Rect rect = { x, y, width, height };
	SDL_BlitSurface(sprite_sheet, &rect, surface, 0);

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_Renderer, surface);
	SDL_FreeSurface(surface);
	return newTexture;
}

SpritesHandler::SpritesHandler() {
	currentGroup = 0;
}

void SpritesHandler::addGroup(std::string sheet_name, int width_per_sprite, int height_per_sprite, int offsetX, int offsetY, int row_index, int nb_of_frames,
	std::string group_name, int speed, SDL_Color alpha) {

	std::vector<SDL_Texture*> texturesVec;

	SDL_Surface* surface = Textures_Manager::findSurface(sheet_name);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, alpha.r, alpha.g, alpha.b));

	for (int i = 0; i < nb_of_frames; i++) {
		SDL_Texture* texture = crop_surface(surface, (offsetX + width_per_sprite) * i + offsetX, (offsetY + height_per_sprite) * row_index + offsetY,
																												width_per_sprite, height_per_sprite);
		texturesVec.push_back(texture);
	}
	groups.push_back(sprite_group(texturesVec, speed, group_name));
}

void SpritesHandler::setIdle(bool idle)
{
	groups[currentGroup].idle = idle;
}



SDL_Texture* SpritesHandler::currentFrame() {
	if (groups[currentGroup].idle) {
		return groups[currentGroup].textures[0];
	}
	int index = (int)((SDL_GetTicks()) / groups[currentGroup].speed) % (groups[currentGroup].textures.size());
	return groups[currentGroup].textures[index];
}

void SpritesHandler::setCurrentGroup(std::string group_name) {
	for (int i = 0; i < groups.size(); i++) {
		if (groups[i].name == group_name) {
			currentGroup = i;
			return;
		}
	}
	std::cout << "ERROR: SPRITE GROUP NOT FOUND (name: " << group_name << ")" << std::endl;
}
