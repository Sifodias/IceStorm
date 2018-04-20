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
	if (!desu.compare("|firstTalkClean"))
		firstTalkClean();

	Character::movingUnit.unlockMovements();
}

int DialogEngine::choiceMode(string a, string b, string c = "", string d = "")
{
	Text_Printer::standStill = 1;
	SDL_Rect bite = { 0, 0, 300, 50 };
	Text_Printer::addToQueue(a, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(b, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(c, &bite, 1);
	bite.y += 10;
	Text_Printer::addToQueue(d, &bite, 1);

	SDL_Event e;
	int out = 0;
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
		SDL_SetRenderDrawColor(Renderer::g_Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		Textures_Manager::blitStuff();
		Text_Printer::handleRoutine(e);
		SDL_RenderPresent(Renderer::g_Renderer);
	}
	return -1;
}


void DialogEngine::firstTalk()
{
	print("Hey stranger ! Want a blowjob ?");
	print("We have many discounts !");
	erabe("Yee", "Nou");
	if (A) {
		print("Well ... that was unexpected ... Alright let's go !");
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


void intro() {
	print("Well, here we are");
	print("Mistakes can't be fixed, can they ?");
	print("It's now time to take responsabilities");
	print("I wish you the best of luck ...");
}

void genny1() {
	print("You're new here, aren't you ?");
	print("I wonder how you get there ...");
	print("But it doesn't matter now");
	print("You shall try to make some friends, if you don't wanna disappear ...");
	print("But keep in mind that it's all lies, in order to stay sane");
	print("I'm out for now");
}