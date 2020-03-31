#include "Objects_Manager.h"
#include "Generic_Object.h"
#include "Text_Printer.h"
#include "Dialog_Engine.h"
#include "Character.h"
#include "Map.h"
using namespace std;

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

string getNextWord(string str) {
	while (str[0] == ' ')
		str.erase(0, 1);

	string word = "";
	int i = 0;
	while (str[i] != ' ' && i < str.size()) {
		word += str[i];
		i++;
	}
	str.erase(0, i);

	while (str[0] == ' ')
		str.erase(0, 1);

	return word;
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
	/*
	if (!type.compare("DOOR")) {
		//check level, front/back and id
		string levelName = getNextWord(content);
		string type = getNextWord(content);
		string id = getNextWord(content);
		if (!levelName.empty() && !type.empty() && !id.empty()) {
			try {
				int doorId = stoi(id);

			}
			catch (exception&) {

			}


		}
	}
	*/
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
