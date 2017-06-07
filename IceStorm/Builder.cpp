#include "Builder.h"
#include <string>

GObject* Builder::currentObject = NULL;

void Builder::fetch()
{
	std::string buffer;
	getline(std::cin, buffer);
	while (buffer.compare("z")) {
		if (buffer.compare(""))

		getline(std::cin, buffer);
	}


	return;
}

void Builder::routine(SDL_Event & e)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_t:
			fetch();
			break;
		}
	}

}
