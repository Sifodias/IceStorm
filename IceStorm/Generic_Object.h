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
#include "Text_Printer.h"

typedef enum t_type {
	BUTTON,
	DIALOG,
	BREAKABLE,
	DOOR,
	GENERIC
	//...
} OTYPE;

class GObject {
public:
	GObject() : ID(0), solid(0), type("GENERIC"), texture(NULL) {};
	int ID;
	std::string target;
	std::vector<std::string> targetnames;
	std::vector<std::string> flags;
	std::string type;
	//OTYPE type;
	SDL_Texture* texture;
	C_Rect rect;
	bool solid;
	std::string content;

	bool flagTrigger; //prevent infinite trigger

	void trigger();
	bool checkFlag(std::string flag);
};