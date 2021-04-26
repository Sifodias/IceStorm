#include "Controller.h"
#include <iostream>
#include <SDL2/SDL.h>

SDL_GameController* Controller::gGameController = NULL;
SDL_Haptic* Controller::gControllerHaptic = NULL;
std::map<std::string, SDL_GameControllerButton> Controller::inputs_controller;
std::map<std::string, SDL_Keycode> Controller::inputs_keyboard;

#define JOYSTICK_DEAD_ZONE 8000
#define JOY_ENABLED true

using namespace std;

void Controller::init() {

	if (SDL_NumJoysticks() < 1) {
		cout << "Warning: No joystick connected!" << endl;
	}
	else {
		//Load joystick
		gGameController = SDL_GameControllerOpen(0);
		if (gGameController == NULL) {
			cout << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << endl;
		}
		else {
			//Get controller haptic device
			SDL_Joystick* JoystickHandle = SDL_GameControllerGetJoystick(gGameController);
			gControllerHaptic = SDL_HapticOpenFromJoystick(JoystickHandle);
			if (gControllerHaptic == NULL) {
				cout << "Warning: Controller does not support haptics! SDL Error: " << SDL_GetError() << endl;
			}
			else {
				//Get initialize rumble
				if (SDL_HapticRumbleInit(gControllerHaptic) < 0) {
					cout << "Warning: Unable to initialize rumble! SDL Error: " << SDL_GetError() << endl;
				}
			}
		}
	}

	//Dummy cfg (todo: need to load from save)
	inputs_controller["use"] = SDL_CONTROLLER_BUTTON_A;
	inputs_controller["up"] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	inputs_controller["down"] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	inputs_controller["left"] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	inputs_controller["right"] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	inputs_controller["jaja"] = SDL_CONTROLLER_BUTTON_Y;

	inputs_keyboard["use"] = SDLK_j;
	inputs_keyboard["up"] = SDLK_w;
	inputs_keyboard["down"] = SDLK_s;
	inputs_keyboard["left"] = SDLK_a;
	inputs_keyboard["right"] = SDLK_d;

}

void Controller::rumbleTest() {
	while (1) {
		if (SDL_HapticRumblePlay(gControllerHaptic, 1, 3000) != 0) {
			cout << "Warning: Unable to play rumble! SDL Error: " << SDL_GetError() << endl;
		}
		SDL_Delay(3200);
	}
}

bool Controller::checkAction(SDL_Event& e, std::string action) {
	bool res = false;
	if (gGameController != NULL) {
		if (e.type == SDL_JOYBUTTONDOWN) {
			auto resCon = inputs_controller.find(action);
			if (resCon != inputs_controller.end()) {
				res = (e.cbutton.button == resCon->second);
			}
		}
		if (e.type == SDL_JOYAXISMOTION && JOY_ENABLED) {
			if (e.jaxis.axis == 1) {
				if (action == "up")
					res = (e.jaxis.value < -JOYSTICK_DEAD_ZONE) || res;

				if (action == "down")
					res = (e.jaxis.value > JOYSTICK_DEAD_ZONE) || res;

			}
			if (e.jaxis.axis == 0) {
				if (action == "left")
					res = (e.jaxis.value < -JOYSTICK_DEAD_ZONE) || res;

				if (action == "right")
					res = (e.jaxis.value > JOYSTICK_DEAD_ZONE) || res;
			}
		}
	}
	if (e.type == SDL_KEYDOWN) {
		auto resKey = inputs_keyboard.find(action);
		if (resKey != inputs_keyboard.end()) {
			res = (e.key.keysym.sym == resKey->second) || res;
		}
	}

	return res;
}

bool Controller::checkActionState(std::string action) {
	bool res = false;
	if (gGameController != NULL) {
		auto resCon = inputs_controller.find(action);
		if (resCon != inputs_controller.end()) {
			res = SDL_GameControllerGetButton(gGameController, resCon->second);
		}
		if (JOY_ENABLED) {
			if (action == "up")
				res = (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY) < -JOYSTICK_DEAD_ZONE) || res;

			if (action == "down")
				res = (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTY) > JOYSTICK_DEAD_ZONE) || res;

			if (action == "left")
				res = (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTX) < -JOYSTICK_DEAD_ZONE) || res;

			if (action == "right")
				res = (SDL_GameControllerGetAxis(gGameController, SDL_CONTROLLER_AXIS_LEFTX) > JOYSTICK_DEAD_ZONE) || res;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	auto resKey = inputs_keyboard.find(action);
	if (resKey != inputs_keyboard.end()) {
		res = state[SDL_GetScancodeFromKey(resKey->second)] || res;
	}

	return res;
}