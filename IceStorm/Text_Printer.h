#pragma once
#include <vector>
#include <SDL.h>
#include <string>
#include <array>
#include "Global_Flags.h"

#define print(a) Text_Printer::addToQueue(a)

typedef struct t_nodequeue {
	std::string str;
	SDL_Rect rect;
	int policeID;
	int iterator;
	SDL_Rect container;
	bool lock;
	bool showDialogBox;

	Uint32 timerWaitA;
	Uint32 timerWaitB;
} NodeQueue;

namespace Text_Printer {
	void init();
	void addToQueue(std::string str, 
		SDL_Rect* container = NULL, int immediate = 0, int policeID = 0, SDL_Rect* rect = NULL, bool showDialogBox = 1);
	void handleRoutine(SDL_Event e);
	void keepGoin(SDL_Event e, std::vector<NodeQueue>& iQueue);

	void flush(int i); //1 for regular queue, 2 for immediate
	void printText(NodeQueue& node);
	void quit();

	extern bool standStill;
	extern std::vector<NodeQueue> queue;
	extern bool busy;
};

/* 
Text formatting :

#<n> = wait of n ms in the middle of a line
Example:  "I love ... #300 you"

*/