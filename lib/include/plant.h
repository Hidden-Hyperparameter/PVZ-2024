#pragma once
#include "unit.h"
class Plant :public Unit {
public:
	bool is_moving_ = true;//whether following mouse
	int life_;
	int max_life_;
public:
	Plant(int xp, int yp, Game* gm, BackGround* bkg, int mxnum, int row = -1,int max_life=Parameters::PLANT_DEFAULT_LIFE);
	void Update()override;
	virtual void Execute();
	void DeduceLife(int amount);
};