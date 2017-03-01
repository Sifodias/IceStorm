//TODO :
//Finish textures engine
//Physics engine
//Dialogues engine
//GH engine
//#include <SDL.h>
#include "Useful_Fonctions.h"

int main(int argc, char* argv[])
{
	initialize_game();
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
	//	"Missing file",
	//	"File is missing. Please reinstall the program.",
	//	NULL);
	main_event_loop();

	//system("pause");
	return 0;
}