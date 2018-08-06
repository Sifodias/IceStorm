#include "Events_Manager.h"
#include "Renderer.h"
#include "Sprites_Handler.h"
#include "Objects_Manager.h"


std::vector<std::function<void()>> Events_Manager::eventsQueue;
bool Events_Manager::busy = 0;

Events_Manager::Events_Manager() {
}

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

}

void testTitle()
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
	GObject* choiceTick = Objects_Manager::findObject("choiceTick");
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
		SDL_RenderCopy(Renderer::g_Renderer, choiceTick->texture, NULL, &choiceRect);
		Text_Printer::handleRoutine(e);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderPresent(Renderer::g_Renderer);
	}
	Text_Printer::flush(2);

	if (choice == 1)
		Renderer::quitAll();
	

}