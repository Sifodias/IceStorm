#include "Events_Manager.h"
#include "Renderer.h"
#include "Sprites_Handler.h"
#include "Objects_Manager.h"
#include "Character.h"
#include "Builder.h"
#include "Text_Printer.h"
#include "Camera.h"

std::vector<std::function<void()>> eventsQueue;
bool busy = 0;

typedef enum {
	TEXT_FLUSHED
} cond;

void Events_Manager::routine()
{
	if (busy)
		return;

	if (!eventsQueue.empty()) {
		eventsQueue.front()();
		eventsQueue.erase(eventsQueue.begin(), eventsQueue.begin() + 1);
	}
}

void Events_Manager::addToQueue(std::function<void()> event)
{
	eventsQueue.push_back(event);
}

int choiceMenu(int numchoices, int xFirstChoice, int offset) {
	return 0;
}

void Events_Manager::testTitle()
{
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
	GObject choiceTick = Objects_Manager::findObject("choiceTick");
	SDL_Rect choiceRect{ container.x - 20,container.y - 32, 5, 10 };
	int lock = 0;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			SDL_FlushEvent(SDL_MOUSEMOTION);			//This useless event overloads the event queue
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
		//SDL_RenderCopy(Renderer::g_Renderer, choiceTick.texture, NULL, &choiceRect);
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
	Objects_Manager::objectsRoutine(e);
	Textures_Manager::printFrame();
	Text_Printer::handleRoutine(e);

	SDL_RenderPresent(Renderer::g_Renderer);
}

void waitLoop(cond c) {
	SDL_Event e;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			SDL_FlushEvent(SDL_MOUSEMOTION);			//This useless event overloads the event queue
			if (e.type == SDL_QUIT) {
				Renderer::quitAll();
				break;
			}
		}

		routinesBlock(e);

		switch (c) {
		case TEXT_FLUSHED:
			if (!Text_Printer::busy) {
				return;
			}
			break;
		}
	}
}


void Events_Manager::floweyCin() {
	//Character::lockMovements(true);
	//Character::textures.setCurrentGroup("up");
	//Character::textures.setIdle(true);

	auto& flowey = Objects_Manager::findObject("flowey_dead");
	flowey.imgIndex = Textures_Manager::findIndex("maindown.png");

	print("I like trains.");

	print("Also #200 .#200 .#200 .#200 I hate you.");
	waitLoop(TEXT_FLUSHED);

	std::vector<int> pelleksID;

	for (int i = 0; i < 4; i++)
		pelleksID.push_back(Objects_Manager::createObject("texture: testc.png, flags: CONTACT DYNAMIC").ID);

	for (int id : pelleksID) {
		GObject& obj = Objects_Manager::findObject(id);
		obj.movingUnit.noclip = true;
		obj.movingUnit.followTarget(Character::movingUnit, 50);
	}

	Objects_Manager::findObject(pelleksID[0]).movingUnit.hitBox = { 110, 264, 5, 5 };
	Objects_Manager::findObject(pelleksID[1]).movingUnit.hitBox = { 135, 242, 5, 5 };
	Objects_Manager::findObject(pelleksID[2]).movingUnit.hitBox = { 170, 242, 5, 5 };
	Objects_Manager::findObject(pelleksID[3]).movingUnit.hitBox = { 201, 264, 5, 5 };

	SDL_Event e;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			SDL_FlushEvent(SDL_MOUSEMOTION);			//This useless event overloads the event queue
			if (e.type == SDL_QUIT) {
				Renderer::quitAll();
				break;
			}
		}
		routinesBlock(e);
		for (int id : pelleksID) {
			if (SDL_HasIntersection(&Character::movingUnit.hitBox.sdl(),
				&Objects_Manager::findObject(pelleksID[0]).movingUnit.hitBox.sdl()))
				goto out;
		}
	}
out:
	for (int id : pelleksID)
		Objects_Manager::deleteObject(id);

	print("I hope it hurts.");
	waitLoop(TEXT_FLUSHED);
	Character::lockMovements(false);
}