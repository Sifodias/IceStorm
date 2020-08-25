#include "Controller.h"
#include <iostream>
#include <SDL.h>

SDL_Joystick* Controller::gGameController = NULL;
SDL_Haptic* Controller::gControllerHaptic = NULL;

using namespace std;

void Controller::init() {

	if (SDL_NumJoysticks() < 1) {
		cout << "Warning: No joysticks connected!\n" << endl;
	}
	else {
		//Load joystick
		gGameController = SDL_JoystickOpen(0);
		if (gGameController == NULL) {
			cout << "Warning: Unable to open game controller! SDL Error: %s\n" << SDL_GetError() << endl;
		}
		else {
			//Get controller haptic device
			gControllerHaptic = SDL_HapticOpenFromJoystick(gGameController);
			if (gControllerHaptic == NULL) {
				cout << "Warning: Controller does not support haptics! SDL Error: %s\n" << SDL_GetError() << endl;
			}
			else {
				//Get initialize rumble
				if (SDL_HapticRumbleInit(gControllerHaptic) < 0) {
					cout << "Warning: Unable to initialize rumble! SDL Error: %s\n" << SDL_GetError() << endl;
				}
			}

		}
	}
}

void Controller::rumbleTest() {
	while (1) {
		if (SDL_HapticRumblePlay(gControllerHaptic, 1, 3000) != 0) {
			cout << "Warning: Unable to play rumble! %s\n" <<  SDL_GetError() << endl;
		}
		SDL_Delay(3200);
	}
}