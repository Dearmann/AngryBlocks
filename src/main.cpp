#include <iostream>
#include "screen.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

int main()
{
	Screen main_screen;

	main_screen.loadScore(SaveType::Binary);

	main_screen.loadLevelsLayout(SaveType::Binary);
	main_screen.loadLevelsLayout(SaveType::Text);

	main_screen.createWindow();

	main_screen.saveScore(SaveType::Binary);

	//_CrtDumpMemoryLeaks();
}