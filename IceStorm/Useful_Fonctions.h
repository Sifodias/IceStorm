#pragma once
#include <fstream>
#include <SDL.h>
void initialize_game();

void main_event_loop();

std::ifstream loadFromTxt(std::string path);	//Load stream from txt