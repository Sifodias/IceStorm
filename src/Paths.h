//this class contains the paths used by the engine
#pragma once
#include <string>
#include <iostream>
#include <fstream>


namespace Paths {
	extern std::string levelPath;
	extern std::string texturesPath;
	extern std::string asciiPath;
	// extern std::string entData;
	extern std::string saveData;
	extern std::string musicPath;
	extern std::string blueprintsPath;
};

std::ifstream* loadFile(std::string path);