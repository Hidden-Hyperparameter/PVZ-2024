// #include <graphics.h>
#include <stdio.h>
#include <time.h>
// #include <conio.h>
#include "lib/include/game.h"
#include "gui/lib/include/app.h"
int main()
{
	// Vulkan::App app;
	// app.run();
	// printf("Hellp,worild!\n");

	// freopen("../../debug.log","w",stdout);
	#ifdef _WIN32
	printf("Program starting at %s\n",__TIME__);
	#endif
	Game g;
	g.Start();
	// Game g;
	// g.Init();
	// while(g.in_game_){
	// 	g.Update();
	// }
	return 0;
}
