#pragma once
#include <fstream>
void initialize_game();

void main_event_loop();

std::ifstream loadFromTxt(std::string path);	//Load stream from txt