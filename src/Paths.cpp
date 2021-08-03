#include "Paths.h"

std::string Paths::levelPath = "../Data/Levels/";
std::string Paths::texturesPath = "../Data/Textures/";
std::string Paths::asciiPath = "../Data/Polices/";
//std::string Paths::entData = "./Data/Entities/ent_data.json";
std::string Paths::blueprintsPath = "../Data/Entities/blueprints.json";
std::string Paths::saveData = "../Data/Saves/"; // save.txt"
std::string Paths::musicPath = "../Data/Music/";

std::ifstream* loadFile(std::string path) {
    std::ifstream* level_stream = new std::ifstream;
    level_stream->open(path.c_str());
    if (!(*level_stream)) {
        std::cout << "Can't load the stream at path : " << path << std::endl;
        level_stream = NULL;
    }
    return level_stream;
}