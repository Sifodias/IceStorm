#include "Text_Printer.h"

std::vector<std::array<SDL_Texture*, 127>> Text_Printer::lettersVec;
std::vector<NodeQueue> Text_Printer::queue;
Uint32 Text_Printer::timerA;
Uint32 Text_Printer::timerB;
SDL_Rect Text_Printer::defaultRect;

void Text_Printer::printText(NodeQueue& node) {
	SDL_Rect blitRect = node.rect;
	blitRect.x -= node.iterator*blitRect.w;
	for (auto i = 0; i <= node.iterator; i++) {
		SDL_RenderCopy(Renderer::g_Renderer, lettersVec[node.policeID][node.str[i]],
			NULL, &blitRect);
		blitRect.x += blitRect.w;
	}
}

void Text_Printer::addToQueue(std::string str, SDL_Rect* rect, int policeID, bool hold)
{
	if (rect == NULL) rect = &defaultRect;
	if (policeID > lettersVec.size() - 1) {
		std::cout << "Invalid index for Police ! index : " << policeID << std::endl;
		return;
	}
	NodeQueue tempNode{ str, *rect, policeID, 0 , hold };
	if (queue.size() > 0) queue[0].hold = 0;
	queue.push_back(tempNode);
}

void Text_Printer::handleRoutine()
{
	timerB = SDL_GetTicks();
	if (queue.size() > 0) {
		if (timerB - timerA > PRINT_SPEED && queue[0].iterator != queue[0].str.size()) {
			printText(queue[0]);
			queue[0].iterator++;
			queue[0].rect.x += queue[0].rect.w;
			timerA = timerB = SDL_GetTicks();
		}
		else printText(queue[0]);

		if (queue[0].iterator == queue[0].str.size() && !queue[0].hold)
			queue.erase(queue.begin(), queue.begin() + 1);
	}
}

void Text_Printer::Init() {
	std::ifstream tempStream = loadFile(Paths::asciiPath);
	if (!tempStream) {
		printf("ERROR : policeList not loaded\n");
		return;
	}

	std::string catcher;
	std::vector<TTF_Font*> tempVec;
	tempStream.clear();
	tempStream.seekg(0);
	while (!tempStream.eof()) {
		getline(tempStream, catcher);
		catcher.insert(0, "./Polices/");
		std::cout << catcher << std::endl;
		tempVec.push_back(TTF_OpenFont(catcher.c_str(), 200));
	}

	char tempStr[2] = "A";
	tempStr[1] = '\0';
	SDL_Color textColor = { 255, 255, 255 };
	SDL_Surface* textSurface = NULL;
	std::array<SDL_Texture*, 127> tempLetters = { NULL };

	for (auto i = tempVec.begin(); i != tempVec.end(); ++i) {
		for (int y = 0; y < 127; y++) {
			tempStr[0] = y;
			textSurface = TTF_RenderText_Solid(*i, tempStr, textColor);
			tempLetters[y] = SDL_CreateTextureFromSurface(Renderer::g_Renderer, textSurface);
		}
		lettersVec.push_back(tempLetters);
	}

	timerA = timerB = SDL_GetTicks();
	defaultRect.h = 8; defaultRect.x = 30;
	defaultRect.w = 8; defaultRect.y = 30;
}