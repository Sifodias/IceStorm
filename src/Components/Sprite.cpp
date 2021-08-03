#include "Sprite.h"
#include "Renderer.h"

SDL_Texture* crop_surface(SDL_Surface* sprite_sheet, int x, int y, int width, int height) {
	SDL_Surface* surface = SDL_CreateRGBSurface(sprite_sheet->flags, width, height, sprite_sheet->format->BitsPerPixel, sprite_sheet->format->Rmask, sprite_sheet->format->Gmask, sprite_sheet->format->Bmask, sprite_sheet->format->Amask);
	SDL_Rect rect = { x, y, width, height };
	SDL_BlitSurface(sprite_sheet, &rect, surface, 0);

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_Renderer, surface);
	SDL_FreeSurface(surface);
	return newTexture;
}

Sprite::Sprite() {
	currentGroup = 0;
	signalDone = false;
}

void Sprite::addGroup(std::string sheet_name, int width_per_sprite, int height_per_sprite, int offsetX, int offsetY, int row_index, int nb_of_frames,
	std::string group_name, int speed, bool playOnce, SDL_Color alpha) {

	std::vector<SDL_Texture*> texturesVec;

	SDL_Surface* surface = Textures_m::findSurface(sheet_name);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, alpha.r, alpha.g, alpha.b));

	for (int i = 0; i < nb_of_frames; i++) {
		SDL_Texture* texture = crop_surface(surface, (offsetX + width_per_sprite) * i + offsetX, (offsetY + height_per_sprite) * row_index + offsetY,
			width_per_sprite, height_per_sprite);
		texturesVec.push_back(texture);
	}
	groups.push_back(sprite_group(texturesVec, speed, group_name, playOnce));
}

void Sprite::setIdle(bool idle) {
	groups[currentGroup].idle = idle;
}

void Sprite::setSingleFrame(std::string textureName) {
	SDL_Texture* tex = Textures_m::findTexture(textureName);
	if (tex == NULL) {
		cout << "No texture found with name: " << textureName << endl;
		return;
	}

	groups.clear();
	currentGroup = 0;

	std::vector<SDL_Texture*> texturesVec = { tex };
	groups.push_back(sprite_group(texturesVec, 0, "single"));
}



SDL_Texture* Sprite::currentFrame() {
	if (groups.size() < currentGroup + 1) {
		return NULL;
	}
	auto& cg = groups[currentGroup];
	if (cg.idle) {
		return cg.textures[0];
	}
	int index = 0;
	if (cg.speed) {
		if (SDL_GetTicks() - cg.timerA > cg.speed) {
			cg.index += 1;
			cg.index = cg.index % cg.textures.size();
			cg.timerA = SDL_GetTicks();
		}
	}

	if (cg.index == cg.textures.size() - 1) {
		signalDone = true;
		groups[currentGroup].done = true;
	}

	if (cg.done && cg.playOnce)
		return cg.textures.back();


	return cg.textures[cg.index];
}

void Sprite::setCurrentGroup(std::string group_name) {
	for (int i = 0; i < groups.size(); i++) {
		groups[i].done = false;
		if (groups[i].name == group_name) {
			//if(currentGroup != i)
			//	groups[i].timerA = SDL_GetTicks();
			currentGroup = i;
		}
	}
	signalDone = false;
	//std::cout << "ERROR: SPRITE GROUP NOT FOUND (name: " << group_name << ")" << std::endl;
}

void Sprite::clear() {
	groups.clear();
}