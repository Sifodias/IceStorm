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

Each object has a TRIGGER function !!!!!
*/

#include <string>
#include <SDL.h>
#include "C_Rect.h"
#include <vector>

typedef enum t_type {
	BUTTON,
	DIALOG
	//...
} OTYPE;

class GObject {
public:
	int ID;
	std::string target;
	std::vector<std::string> targetnames;
	OTYPE type;
	SDL_Texture* texture;
	C_Rect rect;
	bool solid;
	void trigger() {	//trigger every targetnames + print the content
		for (int i = 0; i < targetnames.size(); i++) {
			//find targets
			//trigger
		}
		switch (type) {
		case BUTTON:
			break;
		case DIALOG:
			//print content
			break;
		}
	}
	std::string content;
};