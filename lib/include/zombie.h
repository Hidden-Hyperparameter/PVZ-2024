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
	int explode_status_=0;
	/**
	 * @brief these two variables is used to make sure that the zombie dying image isn't showing too shortly. Only when update times is multiple of DIE_EFFECT_TIME, the zombie would change frame
	 * 
	 */
	const int DIE_EFFECT_TIME=Parameters::ZOMBIE_DIE_PER_FRAME_TIME;
	int die_effect_curr_time=0;
	int MAX_EAT_IMAGE;//number of eating images
	Unit* eating_plant_ = nullptr;
	int FIRE = Parameters::ZOMBIE_EAT;
	/**
	 * @brief in Vulkan, one object can only put one image. So if you want to put two images, you have to specify another Vulkan::Object
	 * @note this variable is set when the zombie is died
	 */
	Vulkan::App::object_t head_id_;
	bool explode_=false;
public:
	/**
	 * @brief Construct a new Zombie object
	 * @note this will immediately specify the zombie as a normal zombie
	 */
	Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int row,int maxlife=Parameters::ZOMBIE_DEFAULT_LIFE);
	Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, const std::string& name, int row,int maxlife=Parameters::ZOMBIE_DEFAULT_LIFE);
	/**
	 * @brief deduce the zombie life
	 * 
	 * @param amount the number of life the zombie is going to be deducted
	 * @param name When it is "explode", the zombie will explode
	 */
	void DeduceLife(int amount,const std::string& name="pea");
	void Update()override;
	void Show()override;
	/**
	 * @brief find the right-most plant. If it is at attack range then eat
	 * 
	 */
	void Attack();
};