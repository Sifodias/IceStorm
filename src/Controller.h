#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>
#include <map>
#include <string>

namespace Controller {
	void init();
	void rumbleTest();
	bool checkAction(SDL_Event& e, std::string);
	bool checkActionState(std::string);		//checks if key corresponding to action is currently pressed
	bool close();

	extern std::map<std::string, SDL_GameControllerButton> inputs_controller;
	extern std::map<std::string, SDL_Keycode> inputs_keyboard;
	
	extern SDL_GameController* gGameController;
	extern SDL_Haptic* gControllerHaptic;
}