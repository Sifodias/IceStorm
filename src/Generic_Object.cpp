#include "Objects_Manager.h"
#include "Generic_Object.h"
#include "Text_Printer.h"
#include "Dialog_Engine.h"
#include "Character.h"
#include "Map.h"
#include "Camera.h"
#include "Events_Manager.h"
#include "Audio_Manager.h"
#include "Renderer.h"
#include "Textures_Manager.h"

using namespace std;

void GObject::routine(SDL_Event& e) {
	if (!enabled)
		return;
	movingUnit.move(e);
}

void GObject::trigger() {
	if (!enabled)
		return;

	if (checkFlag("CONTACT") && contact_triggered && !checkFlag("PERMANENT"))
		return;

	flagTrigger = 1;
	for (string& name : targetnames)
		if (type == "ENABLER")
			Objects_Manager::findObject(name).enabled = true;
		else if (!Objects_Manager::findObject(name).flagTrigger)
			Objects_Manager::findObject(name).trigger();

	if (type == "BUTTON")
		if (checkFlag("CONTACT"))
			if (checkFlag("PRINT"))
				if (!Text_Printer::busy)
					Text_Printer::addToQueue(meta);

	if (type == "DIALOG")
		if (!Text_Printer::busy)
			if (meta[0] == '|')
				DialogEngine::dialogSelector(meta);
			else
				Text_Printer::addToQueue(meta);

	if (type == "DOOR") {
		istringstream iss(meta);
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
			double x, y;
			double* a = checkFlag("LOCKX") ? &x : &Character::movingUnit.hitBox.x;
			double* b = checkFlag("LOCKY") ? &y : &Character::movingUnit.hitBox.y;
			Map::findOccurrence(stoi(target), a, b);

		}
	}

	if (type == "SOUND")
		Audio_Manager::play(meta);

	if (type == "EVENT_SCRIPTED")
		if (meta == "|script1")
			Events_Manager::addToQueue(Events_Manager::floweyCin);

	if (type == "FOLLOWER"){
		if (meta == "eye chara"){
			movingUnit.followTarget(Character::movingUnit, 200, movingUnit.getCoord(), 4);
		}
	}
	contact_triggered = true;
	flagTrigger = 0;
}

bool GObject::checkFlag(string flag) {
	for (string& str : flags)
		if (str == flag)
			return true;

	return false;
}

void GObject::blit(std::tuple<double, double> coord) {
	if (!Textures_Manager::showInvisibleEnts && checkFlag("INV"))
		return;
	SDL_Texture* sprite = textures.currentFrame();
	if (!sprite)
		if (Textures_Manager::showInvisibleEnts)
			sprite = Textures_Manager::findTexture("inv.png");
		else
			return;

	int w, h;
	SDL_QueryTexture(sprite, NULL, NULL, &w, &h);
	SDL_Rect out = { (int)get<0>(coord), (int)get<1>(coord), w, h };

	SDL_RenderCopy(Renderer::g_Renderer, sprite, NULL, &out);
}

void GObject::blit() {
	blit(movingUnit.getCoord(true));
}

void GObject::setCoord(std::tuple<double, double> coord) {
	movingUnit.setCoord(coord);
}

bool GObject::bounded() {
	return levelBound == Map::levelname || !checkFlag("DYNAMIC");
}