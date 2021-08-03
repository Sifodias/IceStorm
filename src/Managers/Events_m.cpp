#include "Events_m.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Objects_m.h"
#include "Character.h"
#include "Builder.h"
#include "Text_Printer.h"
#include "Camera.h"


std::vector<std::function<void()>> eventsQueue;
bool busy = 0;

typedef enum {
	TEXT_FLUSHED
} cond;

void condQuit(SDL_Event& e) {
	if (SDL_PollEvent(&e) != 0) {
		SDL_FlushEvent(SDL_MOUSEMOTION); //This useless event overloads the event queue
		if (e.type == SDL_QUIT) {
			Renderer::quitAll();
			exit(0);
		}
	}
}
void Events_m::routine() {
	if (busy)
		return;

	if (!eventsQueue.empty()) {
		eventsQueue.front()();
		eventsQueue.erase(eventsQueue.begin(), eventsQueue.begin() + 1);
	}
}

void Events_m::addToQueue(std::function<void()> event) {
	eventsQueue.push_back(event);
}

int choiceMenu(int numchoices, int xFirstChoice, int offset) {
	return 0;
}

void Events_m::testTitle() {
	SDL_Rect container{ Renderer::SCREEN_W / 2 - 32, Renderer::SCREEN_H / 2 - 32, 500, 500 };
	Text_Printer::addToQueue("IceStorm", &container, 1, 0, NULL, 0);
	SDL_Event e{};

	Uint32 timerA = SDL_GetTicks();
	Uint32 timerB = SDL_GetTicks();
	while (timerB - timerA < 2000) {
		SDL_RenderClear(Renderer::g_Renderer);
		Text_Printer::handleRoutine(e);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderPresent(Renderer::g_Renderer);
		timerB = SDL_GetTicks();
	}

	Text_Printer::flush(2);

	Text_Printer::addToQueue("NEW GAME", &container, 1, 0, NULL, 0);
	container.y += 32;
	Text_Printer::addToQueue("QUIT", &container, 1, 0, NULL, 0);

	int choice = 0;
	GObject choiceTick = Objects_m::findObject("choiceTick");
	SDL_Rect choiceRect{ container.x - 20, container.y - 32, 5, 10 };
	int lock = 0;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			SDL_FlushEvent(SDL_MOUSEMOTION); //This useless event overloads the event queue
			if (e.type == SDL_QUIT) {
				Renderer::quitAll();
				break;
			}
		}
		if (e.type == SDL_KEYDOWN) {
			if (!lock) {
				if (e.key.keysym.sym == SDLK_DOWN) {
					choice += 1;
					if (choice > 1)
						choice = 0;
				}
				if (e.key.keysym.sym == SDLK_UP) {
					choice -= 1;
					if (choice < 0)
						choice = 1;
				}
				if (e.key.keysym.sym == SDLK_j) {
					break;
				}
				lock = 1;
			}
		}
		if (e.type == SDL_KEYUP)
			lock = 0;

		choiceRect.y = container.y - 32 + choice * 32;
		SDL_RenderClear(Renderer::g_Renderer);
		// SDL_RenderCopy(Renderer::g_Renderer, choiceTick.texture, NULL, &choiceRect);
		Text_Printer::handleRoutine(e);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderPresent(Renderer::g_Renderer);
	}
	Text_Printer::flush(2);

	if (choice == 1)
		Renderer::quitAll();
}

void routinesBlock(SDL_Event& e) {
	SDL_RenderClear(Renderer::g_Renderer);
	Builder::routine(e);
	Character::characterRoutine(e);
	Objects_m::routine(e);
	Textures_m::printFrame();
	Text_Printer::handleRoutine(e);

	SDL_RenderPresent(Renderer::g_Renderer);
}

void whileBlock() {
	SDL_Event e;
	condQuit(e);
	routinesBlock(e);
}

void waitLoop(cond c) {
	while (1) {
		whileBlock();
		switch (c) {
		case TEXT_FLUSHED:
		if (!Text_Printer::busy) {
			return;
		}
		break;
		}
	}
}

void Events_m::floweyCin() {
	//	Character::lockMovements(true);
	//Character::textures.setIdle(true);
	ob(78).textures.addGroup("t1.png", 20, 28, 0, 0, 0, 2, "hey", 60);
	ob(78).textures.setCurrentGroup("hey");

	Character::textures.setSingleFrame("heart.png");
	Character::movingUnit.hitBox.w = Textures_m::findSurface("heart.png")->w;
	Character::movingUnit.hitBox.h = Textures_m::findSurface("heart.png")->h;
	Character::useMainOffsets = false;

	int idFlow = Objects_m::createObject("texture: flowey.png 21 23 3 15 2 8 dancing 125, flags: DYNAMIC").ID;
	ob(idFlow).movingUnit.hitBox = { 150, 290, 21, 21 };
	//ob(idFlow).textures.addGroup("flowey.png", 21, 23, 3, 15, 2, 8, "dancing", 125);
	ob(idFlow).textures.addGroup("flowey.png", 21, 23, 3, 15, 1, 5, "appear", 80, true);
	ob(idFlow).textures.setCurrentGroup("appear");

	/* Wait that the animation is done */
	while (1) {
		whileBlock();
		if (ob(idFlow).textures.signalDone)
			break;
	}

	print("I like trains.");

	print("Also #200 .#200 .#200 .#200 I hate you.");
	waitLoop(TEXT_FLUSHED);

	//Character::lockMovements(false);

	vector<int> wallID;
	for (int a = 0; a < 5; a++) {
		for (int b = 0; b < 5; b++) {
			if (a != 0 && a != 4) {
				if (b != 0 && b != 4) {
					continue;
				}
			}
			wallID.push_back(Objects_m::createObject("texture: heart.png, flags: SOLID DYNAMIC").ID);
			ob(wallID.back()).movingUnit.hitBox = { 110.0 + b * GRID_W, 360.0 + a * GRID_H, GRID_W, GRID_H };
		}
	}

	ob(idFlow).textures.setCurrentGroup("dancing");
	std::vector<int> pelleksID;

	for (int i = 0; i < 4; i++)
		pelleksID.push_back(Objects_m::createObject("flags: CONTACT DYNAMIC").ID);

	for (int id : pelleksID) {
		GObject& obj = Objects_m::findObject(id);
		obj.movingUnit.noclip = true;
		obj.movingUnit.followTarget(Character::movingUnit, 50);
		obj.textures.clear();
		obj.textures.addGroup("bullet.png", 7, 7, 0, 0, 0, 2, "shot", 100);
		obj.textures.setCurrentGroup("shot");
	}

	ob(pelleksID[0]).movingUnit.hitBox = { 110, 264, 7, 7 };
	ob(pelleksID[1]).movingUnit.hitBox = { 135, 242, 7, 7 };
	ob(pelleksID[2]).movingUnit.hitBox = { 170, 242, 7, 7 };
	ob(pelleksID[3]).movingUnit.hitBox = { 201, 264, 7, 7 };

	while (1) {
		whileBlock();
		for (int id : pelleksID) {
			SDL_Rect tempRect1 = Character::movingUnit.hitBox.sdl();
			SDL_Rect tempRect2 = ob(id).movingUnit.hitBox.sdl();
			if (SDL_HasIntersection(&tempRect1, &tempRect2))
				goto out;
		}
	}
out:
	for (int id : pelleksID)
		Objects_m::deleteObject(id);
	for (int id : wallID)
		Objects_m::deleteObject(id);

	print("I hope it hurts.");
	waitLoop(TEXT_FLUSHED);

	Character::textures.addGroup("frisk.png", 19, 29, 5, 21, 0, 4, "down", 250);
	Character::textures.addGroup("frisk.png", 17, 29, 5, 21, 2, 2, "left", 250);
	Character::textures.addGroup("frisk.png", 17, 29, 5, 21, 4, 2, "right", 250);
	Character::textures.addGroup("frisk.png", 19, 29, 5, 21, 6, 4, "up", 250);
	Character::movingUnit.hitBox.w = CHAR_HITBOX_W;
	Character::movingUnit.hitBox.h = CHAR_HITBOX_H;
	Character::useMainOffsets = true;

	//Character::textures.setIdle(false);
	//Character::lockMovements(false);
}

void Events_m::etalonage() {
	SDL_ShowCursor(SDL_DISABLE);
	SDL_Event e;
	std::vector<Uint32> time_values = { 6000, 4000, 2000, 1000, 500 };
	int i = 0;
	while (1) {
		for (int r = 0; r < 256; r += 255) {
			for (int g = 0; g < 256; g += 255) {
				for (int b = 0; b < 256; b += 255) {
					if (SDL_PollEvent(&e) != 0) {
						SDL_FlushEvent(SDL_MOUSEMOTION);
						if (e.type == SDL_QUIT) {
							Renderer::quitAll();
							exit(0);
						}
					}
					if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_j) {
							i = ++i % time_values.size();
						}
						if (e.key.keysym.sym == SDLK_n) {
							i = 0;
						}
					}
					Uint32 timerA = SDL_GetTicks();
					Uint32 timerB = SDL_GetTicks();
					while (timerB - timerA < time_values[i]) {
						if (SDL_PollEvent(&e) != 0) {
							SDL_FlushEvent(SDL_MOUSEMOTION);
							if (e.type == SDL_QUIT) {
								Renderer::quitAll();
								exit(0);
							}
						}
						if (e.type == SDL_KEYDOWN) {
							if (e.key.keysym.sym == SDLK_j) {
								i = ++i % time_values.size();
							}
							if (e.key.keysym.sym == SDLK_n) {
								i = 0;
							}
						}
						timerB = SDL_GetTicks();
					}
					SDL_RenderClear(Renderer::g_Renderer);
					SDL_SetRenderDrawColor(Renderer::g_Renderer, r, g, b, 0);
					SDL_RenderPresent(Renderer::g_Renderer);
				}
			}
		}
	}
}