#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include "game.h"
int main()
{
	Game g;
	g.Init();
	while(g.in_game_){
		g.Update();
	}
	return 0;
}
