#pragma once
#include "plant.h"
#include "pea.h"
class PeaShooter :public Plant {
public:
	const int SHOOT_INTEVAL = 50;
public:
	PeaShooter(int xp, int yp, Game* gm, BackGround* bkg,  int mxnum);
	//void Shoot();
	void Execute()override;
};