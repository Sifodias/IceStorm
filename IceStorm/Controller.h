#pragma once

#include <SDL_haptic.h>


namespace Controller {
	void init();
	void rumbleTest();

	extern SDL_Joystick* gGameController;
	extern SDL_Haptic* gControllerHaptic;
}
