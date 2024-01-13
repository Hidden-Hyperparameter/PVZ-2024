#pragma once
#include "unit.h"
class Pea:public Unit {
public:
	Pea(int xp, int yp, Game* gm, BackGround* bkg, int mxx);
	const int move_step_;
	const int MAX_X;
	void Update()override;
};