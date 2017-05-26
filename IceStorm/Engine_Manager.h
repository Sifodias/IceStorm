#pragma once
#include <fstream>
#include <SDL.h>
void Init_game();

void main_event_loop();

std::ifstream loadFile(std::string path);	//Load stream from txt
