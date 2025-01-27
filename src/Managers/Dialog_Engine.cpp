#include "Dialog_Engine.h"
#include "Text_Printer.h"
#include "Character.h"
#include "Renderer.h"
#include "Controller.h"

int choice;
#define erabe(...) choice = choiceMode(__VA_ARGS__)
#define A choice==1
#define B choice==2
#define C choice==3
#define D choice==4

int waitInput(int bound1 = -1, int bound2 = 255) {
	SDL_Event e;

	while (1) {
		if (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN && Text_Printer::queue.size() == 1) {
				if (bound1 < e.key.keysym.sym && e.key.keysym.sym < bound2 && bound1 != -1) {
					Text_Printer::flush(1);
					Text_Printer::flush(2);
					return e.key.keysym.sym - 48;
				}
				else if (bound1 == -1) {
					if (Controller::checkAction(e, "use")) {
						return 0;
					}
				}
			}
		}
		SDL_RenderClear(Renderer::g_Renderer);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		Textures_m::printFrame();

		Text_Printer::handleRoutine(e);
		SDL_RenderPresent(Renderer::g_Renderer);
	}
	return -1;
}
void meaning1() {
	print("You must find the meaning of life to get through #200 .#200 .#200 .");
	print("Go back.");
	waitInput(-1);
}

void DialogEngine::dialogSelector(string desu) {
	Character::movingUnit.lockMovements(true);

	if (desu == "|deadFlowey")
		firstTalkClean();
	if (desu == "|meaning1")
		meaning1();

	Character::movingUnit.lockMovements(false);
}


int DialogEngine::choiceMode(string a, string b, string c = "", string d = "") {
	Text_Printer::standStill = 1;
	SDL_Rect bite = { 0, 0, 300, 50 };
	SDL_RenderCopy(Renderer::g_Renderer, Character::textures.currentFrame(), NULL, &bite);

	SDL_Event e;
	while (Text_Printer::queue.size() > 1) {
		SDL_PollEvent(&e);

		SDL_RenderClear(Renderer::g_Renderer);
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		Textures_m::printFrame();
		Text_Printer::handleRoutine(e);
		SDL_RenderPresent(Renderer::g_Renderer);
	}

	Text_Printer::addToQueue("1) " + a, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue("2) " + b, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(c.size() > 0 ? "3) " + c : c, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(d.size() > 0 ? "4) " + d : d, &bite, 1);

	return waitInput(48, 53);
}



void DialogEngine::firstTalkClean() {
	print("It's just a dead plant.");
	waitInput();
	//print("How do you feel today ?");
	//erabe("Fine", "Tired");
	//if (A) {
	//	print("That's cool :)");
	//	print("See you around buddy");
	//}
	//else if (B) {
	//	print("Oh I'm sorry to hear that");
	//	print("Well, relax and have fun here");
	//}
}