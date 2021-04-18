#pragma once
#include <fstream>
#include <SDL2/SDL.h>

void init_game();

int main_event_loop();

std::ifstream* loadFile(std::string path);	//Load stream from txt

void handleRoutines(SDL_Event e);

void engineQuit();