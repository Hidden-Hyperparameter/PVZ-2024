#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include "game.h"
int main()
{
	Game g;
	while(true){
		g.Update();
		g.Show();
	}
	return 0;
}
