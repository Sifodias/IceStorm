#include "Engine_Manager.h"

int main(int argc, char* argv[])
{
	Init_game();

	main_event_loop();

	return 0;
}
/*
TODO :
	- Menu handler (Print entries, with select by arrows)
	- Fight Handler
	- Finish save management
	- Need for non generic objects ?
		. Cinematics
		. Ennemies

*/