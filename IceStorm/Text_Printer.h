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
} NodeQueue;

class Text_Printer {
public:
	static void Init();
	static void addToQueue(std::string str, 
		SDL_Rect* container = NULL, int immediate = 0, int policeID = 0, SDL_Rect* rect = NULL, bool showDialogBox = 1);
	static void handleRoutine(SDL_Event e);
	void static keepGoin(SDL_Event e, std::vector<NodeQueue>& iQueue);

	static std::vector<std::array<SDL_Texture*, 127>> lettersVec;
	static std::vector<NodeQueue> queue;
	static std::vector<NodeQueue> imQueue;

	static void flush(int i); //1 for regular queue, 2 for immediate

	static bool standStill;	//the last sentence in queue will stay printed
	static bool busy;

private:
	static void printText(NodeQueue& node);
	static Uint32 timerA;
	static Uint32 timerB;
	static SDL_Rect defaultRect;
	static SDL_Rect defaultContainer;
	static int flagOverflow;
	static SDL_Texture* dialogBox;
	static SDL_Rect dialogRect;
};

