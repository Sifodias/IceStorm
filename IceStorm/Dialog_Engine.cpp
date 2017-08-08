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

	if (!desu.compare("|firstTalk"))
		firstTalk();


	Character::movingUnit.unlockMovements();
}

int DialogEngine::choiceMode(string a, string b, string c = "", string d = "")
{
	Text_Printer::standStill = 1;
	SDL_Event e;
	int out = 0;
	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_1) {
					Text_Printer::flush();
					return 1;
				}
				if (e.key.keysym.sym == SDLK_2) {
					Text_Printer::flush();
					return 2;
				}
				if (e.key.keysym.sym == SDLK_3) {
					Text_Printer::flush();
					return 3;
				}
				if (e.key.keysym.sym == SDLK_4) {
					Text_Printer::flush();
					return 4;
				}
			}
		}
		SDL_RenderClear(Renderer::g_Renderer);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

		Textures_Manager::blitStuff();

		Text_Printer::handleRoutine(e);

		SDL_RenderPresent(Renderer::g_Renderer);

	}

	return -1;
}


void DialogEngine::firstTalk()
{
	print("Hey stranger ! Want a blowjob ?, this sentence is way more longer than I expected so it might result in unexpected behavior");
	print("We have many discounts !");
	print("Just a random sentence");
	print("Just fuck off already !");
	erabe("Yee", "Nou");
	if (A) {
		print("Well ... that was unexpected :) Alright let's go !");
		erabe("Yee", "Eh I was joking !");
		if (A)
			print("Damn I was just fooling around ... You're creeping me out");
		else if (B)
			print("Oh that's too bad ... we could have some fun ...");
	}
	else if (B) {
		print("Oh, maybe next time then");
		erabe("I don't think so", "Yeah, sure");
		if (A)
			print("Farewell ...");
		else if (B)
			print("See you next time <3");
	}
}