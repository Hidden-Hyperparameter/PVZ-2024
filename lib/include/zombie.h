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
	const int DIE_EFFECT_TIME=Parameters::ZOMBIE_DIE_PER_FRAME_TIME;
	int die_effect_curr_time=0;
	int MAX_EAT_IMAGE;//image numbers
	Unit* eating_plant_ = nullptr;
	int FIRE = Parameters::ZOMBIE_EAT;
	Vulkan::App::object_t head_id_;
public:
	Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int row,int maxlife=Parameters::ZOMBIE_DEFAULT_LIFE);Zombie::Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, const std::string& name, int row,int maxlife=Parameters::ZOMBIE_DEFAULT_LIFE);
	void DeduceLife(int amount);
	void Update()override;
	void Show()override;
	void Attack();//eat plant
};