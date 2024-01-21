// #include <graphics.h>
#include <stdio.h>
#include <time.h>
// #include <conio.h>
#include "lib/include/game.h"
#include "gui/lib/include/app.h"
int main()
{
	#ifdef _WIN32
	freopen("../../debug.log","w",stdout);
	printf("Program starting at %s\n",__TIME__);
	#endif
	Game g;
	g.Start();
	// TEST
	return 0;
}
