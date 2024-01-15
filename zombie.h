#pragma once
#include "unit.h"
class Zombie :public Unit {
public:
	int life_;
	int max_life_;
	int left_speed_;
	int die_head_status = 0;//image status
	int die_body_status = 0;//image status
	int eat_image_status = 0;//image status
	int die_explode_status = 0;//image status
	const int MAX_EAT_IMAGE;//image numbers
	Unit* eating_plant_ = nullptr;
	const int FIRE = Parameters::ZOMBIE_EAT;
	Parameters::Peatype type;//last attacked type
public:
	Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int mxnum,int row,int maxlife=100);
	void DeduceLife(int amount,Parameters::Peatype type);
	void Update()override;
	void Show()override;
	void Attack();//eat plant
};