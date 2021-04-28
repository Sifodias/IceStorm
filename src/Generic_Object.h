#pragma once
/*
Each object has :
- ID (int)
- target (string)		//current object
- targetname (string)	//slave object
- type (enum)
- texture (sdl_texture)
- CRect
- flag solid
- Content (differs depending on the type, it can be :
. Message
. Dialogue instance
. Activating mechanisms
. Door : <level> <front/back> <id>
. ...	)
All objects are stored in a .txt, which is loaded into a vector of objects

Each object has a TRIGGER function
*/
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "Moving_Unit.h"
#include "Sprites_Handler.h"

/*
type :
	BUTTON,
	DIALOG,
	BREAKABLE,
	DOOR,
	GENERIC,
	TELEPORT,
	ENEMY,
	CAMBLOCKX,
	CAMBLOCKY,
	PATTERN,  //it's itself a pattern
	DYNAMIC,   //Not present in the tilemap
	SOUND

flags :
	CONTACT,	//triggered when contact
	PERMANENT,	//can be triggered unlimited times
	PATTERN,//follows a pattern
	SOLID,
	INV

*/

class GObject {
public:
	GObject() : ID(0), type("GENERIC"), x(0), y(0), flagTrigger(false), contact_triggered(false), imgIndex(0),
		useSpritesHandler(false) {
	};
	int ID;
	std::string target;
	std::vector<std::string> targetnames;
	std::vector<std::string> flags;
	std::string type;
	
	SpritesHandler textures;
	bool useSpritesHandler;
	std::string textureName;

	int imgIndex; //index in vector of img_struct

	int x;
	int y;
	std::string content;
	Moving_Unit movingUnit;

	void routine(SDL_Event& e);

	bool flagTrigger; //prevent infinite trigger
	bool contact_triggered;

	void trigger();
	bool checkFlag(std::string flag);
};