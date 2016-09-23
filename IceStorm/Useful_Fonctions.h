#pragma once
#include <fstream>
void initialize_game();

void main_event_loop();

std::ifstream loadFromTxt(std::string path);	//charger le flux a partir d'un fichier txt