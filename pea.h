#pragma once
#include "unit.h"
//#include "zombie.h"
#include "game_parameters.h"
class Unit;
//class Zombie;
class Parameters;
class Pea:public Unit {
public:
	const int ATTACK_RANGE = Parameters::PEA_MOVE_STEP;
	const int FIRE = Parameters::PEA_FIRE;
public:
	Pea(int xp, int yp, Game* gm, BackGround* bkg,int row, int mxx);
	const int move_step_;
	const int MAX_X;
	void Update()override;
	void Attack();
};