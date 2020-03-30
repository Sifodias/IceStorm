#include "Sprites_Handler.h"


SpritesHandler::SpritesHandler(std::string str, std::string names, int ispeed) {
	currentGroup = NULL;
	speed = ispeed;
	timerA = timerB = SDL_GetTicks();
	std::istringstream iss(str);
	std::string word;
	int i = 0;
	while (iss >> word) {
		if (!word.compare("|")) {
			i++;
			continue;
		}
		if (i == textures.size()) {
			std::vector<SDL_Texture*> tempy;
			textures.push_back(tempy);
		}
		textures[i].push_back(Textures_Manager::findTexture(word));
	}
	std::istringstream isb(names);
	while (isb >> word) {
		groups.push_back(word);
	}
}

SDL_Texture* SpritesHandler::currentFrame() {
	if (currentGroup == NULL)
		return textures[0][0];

	timerB = SDL_GetTicks();
	return (*currentGroup)[((timerB - timerA) / speed) % (currentGroup->size())];
}

void SpritesHandler::setCurrentGroup(std::string group) {
	int y = 0;
	for (auto i = groups.begin(); i != groups.end(); ++i, y++) {
		if (!i->compare(group)) {
			currentGroup = &textures[y];
			break;
		}
	}
}