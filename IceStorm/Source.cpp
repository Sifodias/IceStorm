#include "Engine_Manager.h"
#include <Windows.h>


int main(int argc, char* argv[])
{
	init_game();

	main_event_loop(NULL);

	return 0;
}

/*
TODO :
	- Need to fix memory leak for img_struct and sprite_group

*/