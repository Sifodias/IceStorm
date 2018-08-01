#include "Engine_Manager.h"
#include <Windows.h>


int main(int argc, char* argv[])
{
	Init_game();

	main_event_loop(NULL);
	


	return 0;
}
/*
TODO :
	- Rework the camera blockers
	- Menu handler (Print entries, with select by arrows)
	- Fight Handler
	- Finish save management
	- Need for non generic objects ?
		. Cinematics
		. Ennemies


Do we need an event queue ?
Let's start:
we display the title
then a menu

*/