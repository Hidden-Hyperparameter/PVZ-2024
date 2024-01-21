#pragma once
#include "unit.h"
class Bullet:public Unit{
public:
    int ATTACK_RANGE;
    int FIRE;
    int move_step_;
	int MAX_X;
    Bullet(int xp, int yp,Game* gm, BackGround* bkg,int row, int mxx,int move_step=Parameters::PEA_MOVE_STEP,int fire=Parameters::PEA_FIRE);
    Bullet(int xp, int yp,Game* gm, BackGround* bkg,int row, int mxx,const std::string& name,int move_step=Parameters::PEA_MOVE_STEP,int fire=Parameters::PEA_FIRE);
    void Update()override;
    /**
     * @brief check whether zombies are in attack range and then call zombie deduce life
     * 
     */
	virtual void Attack();
};