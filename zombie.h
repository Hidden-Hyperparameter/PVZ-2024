#pragma once
#include "unit.h"
class Zombie :public Unit {
public:
	int life_;
	int max_life_;
	int left_speed_;
	int die_head_status = 0;
	int die_body_status = 0;
	int eat_image_status = 0;
	const int MAX_EAT_IMAGE;
	Unit* eating_plant_ = nullptr;
	const int FIRE = Parameters::ZOMBIE_EAT;
public:
	Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int mxnum,int row,int maxlife=100);
	void DeduceLife(int amount);
	void Update()override;
	void Show()override;
	void Attack();
};