#pragma once
#include "unit.h"
//#include "zombie.h"
#include "bullet.h"
#include "game_parameters.h"
class Unit;
class Bullet;
//class Zombie;
class Parameters;
class Pea:public Bullet {
public:
public:
	Pea(int xp, int yp,Game* gm, BackGround* bkg,int row, int mxx);
};