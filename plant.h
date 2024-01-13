#pragma once
#include "unit.h"
class Plant :public Unit {
public:
	Plant(int xp, int yp, Game* gm, BackGround* bkg,  int mxnum);
	void Update()override;
	virtual void Execute();
	bool is_moving_ = true;
};