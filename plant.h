#pragma once
#include "unit.h"
class Plant :public Unit {
public:
	bool is_moving_ = true;//whether following mouse
	int life_;
	int max_life_;
	Parameters::Peatype type;//type refers to the bullet type including single range none
	//we verify range as exploding attack here
public:
	Plant(int xp, int yp, Game* gm, BackGround* bkg, int mxnum, int row = -1,int max_life=Parameters::PLANT_DEFAULT_LIFE);
	void Update()override;
	virtual void Execute();
	void DeduceLife(int amount);
};