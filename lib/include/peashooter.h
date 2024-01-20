#pragma once
#include "plant.h"
#include "pea.h"
class Pea;
class PeaShooter :public Plant {
public:
	const int SHOOT_INTEVAL = Parameters::PEA_SHOOT_INTEVAL;
	int pea_shoot_status = 0;
public:
	PeaShooter(int xp, int yp, Game* gm, BackGround* bkg, int row = -1);
	//void Shoot();
	void Execute()override;
};