#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"

/*
type :
	BUTTON,
	DIALOG,
	DOOR,
	GENERIC,
	TELEPORT,
	DYNAMIC,   //created in-game
	SOUND,
	ENABLER,
	FOLLOWER

flags :
	CONTACT,	//triggered when contact
	PERMANENT,	//can be triggered unlimited times
	PATTERN,	//follows a pattern
	SOLID,
	INV,
	LOCKX,
	LOCKY,

*/

class GObject {
public:
	GObject() : ID(0), type("GENERIC"), flagTrigger(false), contact_triggered(false),
		enabled(true), default_enabled(true), textures(), useMUnit(false) {
	};
	int ID;
	std::string target;
	std::vector<std::string> targetnames;
	std::vector<std::string> flags;
	std::string type;

	SpritesHandler textures;

	bool enabled;
	bool default_enabled;
	bool useMUnit;
	std::string meta;
	std::string levelBound;
	Moving_Unit movingUnit;

	void routine(SDL_Event& e);

	bool flagTrigger; //prevent infinite trigger
	bool contact_triggered;

	void trigger();
	bool checkFlag(std::string flag);
	void setCoord(std::tuple<double, double> coord);
	void blit(std::tuple<double, double> coord);
	void blit();
	bool bounded();
};