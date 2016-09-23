#pragma once
#include <SDL.h>
#include <string>
class Renderer {
public:
	//Renderer();

	static void initAll();		//Initialiser les modules de la SDL
	static void quitAll();		//Decharger les modules de la SDL

	void renderObjects();
	
	static SDL_Renderer* g_Renderer;	//le Renderer	(static pour plus de flexibilité, etre utilisé sans objet
	static SDL_Window* g_Window;		//la fenetre

};