#include "Objects_Manager.h"
#include "Generic_Object.h"
#include "Text_Printer.h"
#include "Dialog_Engine.h"
#include "Character.h"
#include "Map.h"
#include "Camera.h"

//using namespace std;

void GObject::routine()
{
	if (!type.compare("ENEMY")) {
		for (int i = 0; i < flags.size(); i++) {
			if (!flags[i].compare("PATTERN")) {
				switch (movingUnit.mainDirection) {
				case 1:
					if (!Objects_Manager::findObject(std::to_string
					(Map::getIdObject(movingUnit.hitBox.y + movingUnit.hitBox.h + 1, 0, movingUnit.hitBox.x, 0)))
						.type.compare("PATTERN")) {
						movingUnit.speedY = movingUnit.move_speed;
					}
					break;
				case -1:

					break;

				case 2:

					break;

				case -2:

					break;
				}


				break;
			}
		}
	}
	//movingUnit.move();
}


void GObject::trigger()
{
	flagTrigger = 1;
	for (int i = 0; i < targetnames.size(); i++) {
		if (!Objects_Manager::findObject(targetnames[i]).flagTrigger)
			Objects_Manager::findObject(targetnames[i]).trigger();
	}
	if (!type.compare("BUTTON")) {
		if (checkFlag("CONTACT")) {
			if (checkFlag("PRINT"))
				if (!Text_Printer::busy)
					Text_Printer::addToQueue(content);

		}
	}

	if (!type.compare("DIALOG")) {
		if (!Text_Printer::busy) {
			if (content[0] == '|') {
				DialogEngine::dialogSelector(content);
			}
			else
				Text_Printer::addToQueue(content);
		}
	}

	if (!type.compare("TELEPORT")) {
		Character::movingUnit.teleport(x, y);
	}
	
	if (!type.compare("DOOR")) {
		istringstream iss(content);
		string word;
		iss >> word;

		/* Check if it's an entry or exit */
		if (word == "0") {
			/* Get the door target */
			iss >> word;
			string target = word;

			/* Get the destination level */
			iss >> word;

			/* Teleportation */
			Map::loadLevel(word);
			Map::findOccurrence(stoi(target), &Character::movingUnit.hitBox.x, &Character::movingUnit.hitBox.y);
			if (word == "flowey") {
				Camera::lockCamX(160, true);
			}
			else {
				Camera::lockCamX(0, false);
			}
		}

	}
	
	/*
	if (!type.compare("CAMBLOCK")) {

	}
	*/

	flagTrigger = 0;
}

bool GObject::checkFlag(string flag)
{
	for (auto i = flags.begin(); i != flags.end(); ++i) {
		if (!i->compare(flag)) return true;
	}
	return false;
}
