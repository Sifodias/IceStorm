#pragma once
#include <fstream>
#include <SDL.h>
void Init_game();

int main_event_loop(void*);

std::ifstream* loadFile(std::string path);	//Load stream from txt


void handleRoutines(SDL_Event e);

void engineQuit();