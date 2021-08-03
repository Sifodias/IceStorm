#include "Save_m.h"
#include "Objects_m.h"
#include "Map.h"
#include "Character.h"
#include "Paths.h"
#include <string>
using namespace std;

ifstream* saveFile = NULL;



void Save_m::loadSave() {
    saveFile = loadFile(Paths::saveData + "save.txt");

    std::string buffer;
    saveFile->seekg(0);
    std::getline(*saveFile, buffer);

    while (buffer.compare("EOF")) {
        if (Objects_m::identify(buffer, "map: ")) {
            Map::loadLevel(buffer);
            Map::findOccurrence(69, &Character::movingUnit.hitBox.x, &Character::movingUnit.hitBox.y);
        }

        /*if (Objects_m::identify(buffer, "charaMark: ")) {
            GObject tempObj = Objects_m::findObject(buffer);
            movingUnit.teleport(tempObj.x, tempObj.y);
            charaMark = buffer;
            goto next;
        }*/

        std::getline(*saveFile, buffer);
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