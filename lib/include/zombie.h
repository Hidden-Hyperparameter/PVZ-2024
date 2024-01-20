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
	int MAX_EAT_IMAGE;//image numbers
	Unit* eating_plant_ = nullptr;
	int FIRE = Parameters::ZOMBIE_EAT;
public:
	Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int row,int maxlife=Parameters::ZOMBIE_DEFAULT_LIFE);Zombie::Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, const std::string& name, int row,int maxlife=Parameters::ZOMBIE_DEFAULT_LIFE);
	void DeduceLife(int amount);
	void Update()override;
	void Show()override;
	void Attack();//eat plant
};