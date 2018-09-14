#include "Dialog_Engine.h"
#include "Text_Printer.h"
#include "Character.h"
#include "Renderer.h"

int choice;
#define erabe(...) choice = choiceMode(__VA_ARGS__)
#define A choice==1
#define B choice==2
#define C choice==3
#define D choice==4

void DialogEngine::dialogSelector(string desu)
{
	Character::movingUnit.lockMovements();

	if (!desu.compare("|firstTalkClean"))
		firstTalkClean();

	Character::movingUnit.unlockMovements();
}

int DialogEngine::choiceMode(string a, string b, string c = "", string d = "")
{
	Text_Printer::standStill = 1;
	SDL_Rect bite = { 0, 0, 300, 50 };

	SDL_Event e;
	while (Text_Printer::queue.size() > 1) {
		SDL_PollEvent(&e);

		SDL_RenderClear(Renderer::g_Renderer);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 100, 50, 100, SDL_ALPHA_OPAQUE);
		Textures_Manager::blitStuff();
		Text_Printer::handleRoutine(e);
		SDL_RenderPresent(Renderer::g_Renderer);
	}

	Text_Printer::addToQueue(a, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(b, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(c, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(d, &bite, 1);

	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN && Text_Printer::queue.size() == 1) {
				if (48 < e.key.keysym.sym && e.key.keysym.sym < 53) {
					Text_Printer::flush(1);
					Text_Printer::flush(2);
					return e.key.keysym.sym - 48;
				}
			}
		}
		SDL_RenderClear(Renderer::g_Renderer);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 100, 50, 100, SDL_ALPHA_OPAQUE);
		Textures_Manager::blitStuff();
		Text_Printer::handleRoutine(e);
		SDL_RenderPresent(Renderer::g_Renderer);
	}
	return -1;
}



void DialogEngine::firstTalkClean()
{
	print("Hello there !");
	print("How do you feel today ?");
	erabe("Fine", "Tired");
	if (A) {
		print("That's cool :)");
		print("See you around buddy");
	}
	else if (B) {
		print("Oh I'm sorry to hear that");
		print("Well, relax and have fun here");
	}
}