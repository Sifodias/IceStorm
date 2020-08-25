#include "Engine_Manager.h"
#include <Windows.h>


int main(int argc, char* argv[])
{
	init_game();
	
	main_event_loop(NULL);

	return 0;
}
