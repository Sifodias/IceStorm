#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <ostream>
#include "Useful_Fonctions.h"
#include <iostream>
#include "Textures_Manager.h"
#include "Renderer.h"
#include "Paths.h"
#include <array>
#include "Map.h"

class Text_Printer {
public:
	//static void addPolice(std::string name);
	static void Init() {
		std::ifstream tempStream = loadFromTxt(Paths::asciiPath);
		if (!tempStream) {
			printf("ERROR policeList not loaded\n");
			return;
		}

		std::string catcher;
		std::vector<TTF_Font*> tempVec;
		tempStream.clear();
		tempStream.seekg(0);
		while (!tempStream.eof()) {
			getline(tempStream, catcher);
			catcher.insert(0, "./Polices/");
			std::cout << catcher << std::endl;
			tempVec.push_back(TTF_OpenFont(catcher.c_str(), 200)); //28 ?
		}

		char tempStr[2] = "A";
		tempStr[1] = '\0';
		SDL_Color textColor = { 255, 255, 255 };
		SDL_Surface* textSurface = NULL;
		std::array<SDL_Texture*, 127> tempLetters = { NULL };

		for (auto i = tempVec.begin(); i != tempVec.end(); ++i) {
			for (int y = 0; y < 127; y++) {
				tempStr[0] = y;
				textSurface = TTF_RenderText_Solid(*i, tempStr, textColor);
				tempLetters[y] = SDL_CreateTextureFromSurface(Renderer::g_Renderer, textSurface);
			}
			lettersVec.push_back(tempLetters);
		}
	}

	static void printText(std::string str, int index, SDL_Rect blitty) {
		if (index > lettersVec.size()-1){
			std::cout << "Invalid index for Police ! index : " << index << std::endl;
			return;
		}

		for (auto i = 0; i < str.length(); i++) {
			SDL_RenderCopy(Renderer::g_Renderer, lettersVec[index][str[i]], NULL, &blitty);
			blitty.x += blitty.w;
		}
	}
	static std::vector<std::array<SDL_Texture*, 127>> lettersVec;
};
