#include "Objects_Manager.h"
#include "Generic_Object.h"
#include "Text_Printer.h"
#include "Dialog_Engine.h"
#include "Character.h"
#include "Map.h"
#include "Camera.h"
#include "Events_Manager.h"
#include "Audio_Manager.h"

void GObject::routine(SDL_Event& e)
{
	movingUnit.move(e);
}


void GObject::trigger()
{
	if (checkFlag("CONTACT") && contact_triggered && !checkFlag("PERMANENT")) {
		return;
	}

	flagTrigger = 1;
	for (string& name : targetnames) {
		if (!Objects_Manager::findObject(name).flagTrigger)
			Objects_Manager::findObject(name).trigger();
	}
	if (type == "BUTTON") {
		if (checkFlag("CONTACT")) {
			if (checkFlag("PRINT"))
				if (!Text_Printer::busy)
					Text_Printer::addToQueue(content);

		}
	}

	if (type == "DIALOG") {
		if (!Text_Printer::busy) {
			if (content[0] == '|') {
				DialogEngine::dialogSelector(content);
			}
			else
				Text_Printer::addToQueue(content);
		}
	}

	if (type == "DOOR") {
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

	if (type == "SOUND"){
		Audio_Manager::play(content);
	}

	if (type == "EVENT_SCRIPTED") {
		if (content == "|script1") {
			Events_Manager::addToQueue(Events_Manager::floweyCin);
		}
	}
	contact_triggered = true;
	flagTrigger = 0;
}

bool GObject::checkFlag(string flag)
{
	for (string& str : flags) {
		if (str == flag) {
			return true;
		}
	}
	return false;
}
