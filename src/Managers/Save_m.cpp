#include "Save_m.h"
#include "Objects_m.h"
#include "Character.h"
#include "Paths.h"
#include <string>
#include "Objects_m.h"

using namespace std;

ifstream* saveFile = NULL;

std::string Save_m::levelName;

void Save_m::loadSave() {
    saveFile = loadFile(Paths::saveData + "save.txt");

    std::string buffer;
    saveFile->seekg(0);

    while (std::getline(*saveFile, buffer)) {
        if (Objects_m::identify(buffer, "map: ")) {
            levelName = buffer;
            Objects_m::loadLevel(buffer);
            // Map::findOccurrence(69, &Character::movingUnit.hitBox.x, &Character::movingUnit.hitBox.y);
        }
    }
}

void Save_m::save() {
    //write all the fields loaded by loadSavem in the saveFile
}

/*
Content of a save file :
map = Map where it was saved
charaMark = Last character marker



*/