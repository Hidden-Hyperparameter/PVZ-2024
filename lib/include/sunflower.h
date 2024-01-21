#pragma once
#include "plant.h"
#include "game_parameters.h"
class SunFlower :public Plant {
public:
	int gen_sun_time_;
	const int GEN_SUN_INTERVAL = Parameters::SUNFLOWER_GEN_SUN;
public:
	SunFlower(int xp, int yp, Game* gm, BackGround* bkg,int row=-1,int max_life=Parameters::PLANT_DEFAULT_LIFE);
	void Execute()override;
};