#pragma once
#include "unit.h"
class Plant :public Unit {
public:
	/**
	 * @brief this variable denotes whether the plant is following the mouse
	 * 
	 */
	bool is_moving_ = true;
	int life_;
	int max_life_;
public:
	Plant(int xp, int yp, Game* gm, BackGround* bkg, int row = -1,int max_life=Parameters::PLANT_DEFAULT_LIFE);
	Plant(int xp, int yp, Game* gm, BackGround* bkg, const std::string &name,int row = -1,int max_life=Parameters::PLANT_DEFAULT_LIFE);
	void Update()override;
	/**
	 * @brief this function calls the specific attack of plant
	 * @note in the case that the plant attack through a Bullet type, this only check whether there is one zombie and push the bullet in. The task of checking whether the zombie is attacked is given to the Bullet class.
	 */
	virtual void Execute();
	/**
	 * @brief deduce life when eaten by zombie
	 * 
	 * @param amount zombie bite amount
	 */
	void DeduceLife(int amount);
};