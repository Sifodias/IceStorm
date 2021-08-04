#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <string>
#include <iostream>

class img_struct {
public:
	virtual void destroy() {};
	virtual void* data() { return NULL; };
	virtual std::string name() {
		std::cout << "THIS SHOULD NEVER BE PRINTED, EVER" << std::endl;
		return "";
	};
};

class img_opengl : public img_struct {
public:
	img_opengl() {}
};

class img_sdl : public img_struct {
public:
	img_sdl(SDL_Texture* t, SDL_Surface* s, std::string n) : texture(t), surface(s), n(n) {}
	void destroy() {
		if (texture)
			SDL_DestroyTexture(texture);
		if (surface)
			SDL_FreeSurface(surface);
	}
	std::string name() { return n; }

	SDL_Texture* texture;
	SDL_Surface* surface;
	std::string n;
};

namespace Textures_m {
	void init(std::string str = "");	//Loads all the images
	void quit();
	void printFrame();

	SDL_Texture* findTexture(std::string name);
	SDL_Surface* findSurface(std::string name);
	int findIndex(std::string name);

	extern bool showInvisibleEnts;
	extern bool showGrid;
	extern std::vector<img_struct*> imgList;
};
