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
- Content (differs depending on type, which can be :
. Message
. Dialogue instance
. Activating mechanisms
. ...	)
All objects are stored in a .txt, which is loaded into a vector of objects

Each object has a TRIGGER function
*/
#include <string>
#include <SDL.h>
#include "C_Rect.h"
#include <vector>
#include "Moving_Unit.h"

/*
type :
	BUTTON,
	DIALOG,
	BREAKABLE,
	DOOR,
	GENERIC,
	TELEPORT,
	ENEMY,
	PATTERN  //it's itself a pattern

flags :
	CONTACT,	//triggered when contact
	PATTERN,//follows a pattern
	SOLID

*/

class GObject {
public:
	GObject() : ID(0), type("GENERIC"), texture(NULL), x(0), y(0) {};
	int ID;
	std::string target;
	std::vector<std::string> targetnames;
	std::vector<std::string> flags;
	std::string type;
	SDL_Texture* texture;
	std::string textureName;
	int x;
	int y;
	C_Rect rect;
	std::string content;
	Moving_Unit movingUnit;

	void routine();

	bool flagTrigger; //prevent infinite trigger

	void trigger();
	bool checkFlag(std::string flag);
};