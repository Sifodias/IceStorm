#include "Text_Printer.h"
#include <SDL2/SDL_ttf.h>
#include <ostream>
#include <iostream>
#include "Textures_m.h"
#include "Renderer.h"
#include "Paths.h"
#include "Controller.h"
#include <filesystem>

std::vector<std::array<SDL_Texture*, 127>> lettersVec;
std::vector<NodeQueue> Text_Printer::queue;
std::vector<NodeQueue> imQueue;
Uint32 timerA;
Uint32 timerB;
SDL_Rect defaultRect;
SDL_Rect defaultContainer;
int flagOverflow;
SDL_Texture* dialogBox = NULL;
SDL_Rect dialogRect;
bool Text_Printer::busy = 0;
bool Text_Printer::standStill = 0;

void Text_Printer::init() {
	std::vector<TTF_Font*> tempVec;
	for (const auto& entry : std::filesystem::directory_iterator(Paths::asciiPath)) {
		if (!std::filesystem::is_regular_file(entry) || entry.path().extension() != ".fon")
			continue;

		tempVec.push_back(TTF_OpenFont(entry.path().c_str(), 200));
	}

	char tempStr[2] = "A";
	tempStr[1] = '\0';
	SDL_Color textColor = { 255, 255, 255 };
	SDL_Surface* textSurface = NULL;
	std::array<SDL_Texture*, 127> tempLetters = { NULL };

	for (TTF_Font* i : tempVec) {
		for (int y = 0; y < 127; y++) {
			tempStr[0] = y;
			textSurface = TTF_RenderText_Solid(i, tempStr, textColor);
			tempLetters[y] = SDL_CreateTextureFromSurface(Renderer::g_Renderer, textSurface);
			SDL_FreeSurface(textSurface);
		}
		lettersVec.push_back(tempLetters);
		TTF_CloseFont(i);
	}

	timerA = timerB = SDL_GetTicks();
	defaultRect.h = 8; defaultRect.x = 30;
	defaultRect.w = 8; defaultRect.y = 30;
	defaultContainer.h = 28; defaultContainer.x = 20; // For icon included, 79;
	defaultContainer.w = 220; defaultContainer.y = 208;
	flagOverflow = 0;
	dialogBox = Textures_m::findTexture("dialog_box_clean.png");
	dialogRect.x = 0; dialogRect.y = 200;
	dialogRect.h = 40; dialogRect.w = 320;
}

void Text_Printer::printText(int index, bool im = false) {
	auto& node = im ? imQueue[index] : queue[index];
	SDL_Rect blitRect = node.container;
	blitRect.w = node.rect.w; blitRect.h = node.rect.h;
	for (auto i = 0; i < node.iterator; i++) {
		if (i >= node.str.size()) {
			std::cout << "Text printer Error" << std::endl;
		}
		/* Skip the wait time codes */
		while (node.str[i] == '#') {
			i++;
			while (node.str[i] != ' ' && node.str[i] != '\0') {
				i++;
			}
		}

		//check if there is enough space for the word, if not we jump a line
		if (i == 0 || node.str[i] == ' ') {
			int y = i;
			while (node.str[y] == ' ') y++;
			while (node.str[y] != ' ' && node.str[y] != '\0') y++;

			//if out of limit
			if (blitRect.x + ((y - i) * blitRect.w) > node.container.w + node.container.x) {
				if (blitRect.y + 2 * blitRect.h > node.container.h + node.container.y) {
					node.lock = 1;
					return;
				}
				//jump line
				else {
					blitRect.y += blitRect.h;
					blitRect.x = node.container.x;
					while (node.str[i] == ' ') {
						i++;
						if (i > node.iterator)
							break;
					}
				}
			}
		}

		if (node.str[i] < 0 || node.str[i] > 127) {
			SDL_RenderCopy(Renderer::g_Renderer, lettersVec[node.policeID]['?'],
				NULL, &blitRect);
		}
		else SDL_RenderCopy(Renderer::g_Renderer, lettersVec[node.policeID][node.str[i]],
			NULL, &blitRect);
		blitRect.x += blitRect.w;
	}
	if (node.str[node.iterator] == '\0') node.lock = true;
}

void Text_Printer::addToQueue(std::string str,
	SDL_Rect* container, int immediate, int policeID, SDL_Rect* rect, bool showDialogBox) {
	if (!str.size()) return;
	if (rect == NULL) rect = &defaultRect;
	if (container == NULL) container = &defaultContainer;
	if (policeID > lettersVec.size() - 1) {
		std::cout << "Invalid index for Police ! index : " << policeID << std::endl;
		return;
	}
	NodeQueue tempNode{ str, *rect, policeID, 0 , *container, 0, showDialogBox, 0, 0 };
	if (!immediate)
		queue.push_back(tempNode);
	else imQueue.push_back(tempNode);
}

void Text_Printer::keepGoin(SDL_Event e, std::vector<NodeQueue>& q) {
	if (q.size() > 0) {
		if (&q == &queue)
			busy = 1;

		int i = 0;
		int index = 0;
		for (NodeQueue& node : q) {
			printText(index++);
			if (node.lock) {
				if ((&q == &queue) && (!standStill || q.size() > 1)) {
					if (Controller::checkAction(e, "use")) {
						while (node.str[node.iterator] == ' ')
							node.iterator++;

						node.str.erase(0, node.iterator);
						node.lock = false;
						node.iterator = 0;

						if (node.str.empty()) {
							q.erase(q.begin() + i);
							continue;
						}
					}
				}
			}
			if (node.str[node.iterator] == '#') {
				string timeBuf;
				node.iterator++;
				while (node.str[node.iterator] != ' ' && node.str[node.iterator] != '\0')
					timeBuf += node.str[node.iterator++];

				Uint32 waitTime = 0;
				try {
					waitTime = stoi(timeBuf);
				}
				catch (std::exception&) {
					std::cout << "Wait time mentionned is not a number: " << timeBuf << endl;
				}
				node.timerWaitA = waitTime;
			}
			if (node.timerWaitA != 0) {
				Uint32 delta = SDL_GetTicks() - node.timerWaitB;
				node.timerWaitB = SDL_GetTicks();
				node.timerWaitA = std::max(0, (int)node.timerWaitA - (int)delta);
			}

			else if (timerB - timerA > PRINT_SPEED && node.iterator != node.str.size()) {
				node.iterator++;
				timerA = timerB = SDL_GetTicks();
			}


			if (&q == &queue) return;	//We should print the first node in the queue only

			i++;
		}
	}
	else if (&q == &queue) busy = 0;
}

void Text_Printer::handleRoutine(SDL_Event e) {
	timerB = SDL_GetTicks();
	if (queue.size() > 0) {
		if (queue.front().showDialogBox) {
			SDL_RenderCopy(Renderer::g_Renderer, dialogBox,
				NULL, &dialogRect);
			//SDL_Rect bite = { 2, 202, 55, 36 };
			//SDL_RenderCopy(Renderer::g_Renderer, Textures_m::findTexture("testc.png"), NULL, &bite);
		}
	}
	keepGoin(e, queue);
	keepGoin(e, imQueue);
}

void Text_Printer::flush(int i) {
	if (i == 1) {
		queue.erase(queue.begin(), queue.end());
		standStill = 0;
	}
	if (i == 2) {
		imQueue.erase(imQueue.begin(), imQueue.end());
	}
}

void Text_Printer::quit() {
	for (std::array<SDL_Texture*, 127> &police : lettersVec) {
		for (SDL_Texture* texture : police) {
			SDL_DestroyTexture(texture);
		}
	}
}