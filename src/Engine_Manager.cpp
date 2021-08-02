#include "Engine_Manager.h"
#include "Renderer.h"
#include "Map.h"
#include "Textures_Manager.h"
#include "Character.h"
#include <iostream>
#include "Text_Printer.h"
#include "Objects_Manager.h"
#include "Camera.h"
#include "Builder.h"
#include "Events_Manager.h"
#include "Controller.h"
#include "Audio_Manager.h"
#include "Editor.h"
#include "imgui_impl_sdl.h"


void init_game() {
	Renderer::initAll();
	Textures_Manager::init();
	Objects_Manager::init();
	Text_Printer::init();
	Camera::init();
	Character::init();
	Controller::init();
	Audio_Manager::init();
	Editor::init();

	// Events_Manager::addToQueue(Events_Manager::testTitle);
}

void handleRoutines(SDL_Event& e) {
	SDL_RenderClear(Renderer::g_Renderer);
	
	Uint32 timerA;
	Uint32 timerB;
	timerA = timerB = SDL_GetTicks();

	ImGuiIO& io = ImGui::GetIO();
	Controller::blockInput = io.WantCaptureKeyboard;
	if (!io.WantCaptureKeyboard) {
		Builder::routine(e);
	}

	Character::characterRoutine(e);
	Objects_Manager::objectsRoutine(e);
	Events_Manager::routine();
	Textures_Manager::printFrame();
	Editor::routine(e);
	Text_Printer::handleRoutine(e);

	timerB = SDL_GetTicks();
	// std::cout << timerB - timerA << std::endl;
	timerA = timerB = SDL_GetTicks();

	SDL_RenderPresent(Renderer::g_Renderer);
}

int main_event_loop() {
	SDL_Event e;
	int c = 0;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			SDL_FlushEvent(SDL_MOUSEMOTION); //This useless event overloads the event queue
			if (e.type == SDL_QUIT) {
				Renderer::quitAll();
				break;
			}

			ImGui_ImplSDL2_ProcessEvent(&e); //Needs to be called only if the event is new
		}
		// if (Controller::checkAction(e, "jaja"))
		// 	Controller::rumbleTest();
		

		handleRoutines(e);

	}
	return 0;
}

std::ifstream* loadFile(std::string path) {
	std::ifstream* level_stream = new ifstream;
	level_stream->open(path.c_str());
	if (!(*level_stream)) {
		std::cout << "Can't load the stream at path : " << path << std::endl;
		level_stream = NULL;
	}
	return level_stream;
}

void engineQuit() {
	if (SAVE_ENABLED) {
		Map::saveMatrix();
		Objects_Manager::saveObjects();
	}
	Text_Printer::quit();
	Textures_Manager::quit();
	Audio_Manager::quit();
}
