#pragma once
#ifndef __PARAMETERS__
#define __PARAMETERS__
class Parameters {
public://general
	static const int SUN_TIME = 10000;		//time interval for generating sun
	static const int INIT_ZOMBIE_TIME = 5000;       //initial time for begin generating zombie
	static const int ZOMBIE_TIME = 25000;		//time interval for generating zombie
	static const int SUN_STEPS = 400;			//total steps for sun to fall down. The more this is, the slower the sun will fall
	static const int ZOMBIE_STEPS =400; //total steps for zombie to reach left side
	static const int UPDATE_TIME = 100;		//(physical) time interval between updates, unit is milliseconds
	static const int PLANT_DEFAULT_LIFE = 100;		//normal plant life
	static const int ZOMBIE_DEFAULT_LIFE = 100;		//normal zombie life
public://plant
	//peashooter
	static const int PEA_SHOOT_INTEVAL = 10;
	static const int PEA_FIRE = 20;
	static const int PEA_MOVE_STEP = 25;
	//sunflower
	static const int SUNFLOWER_GEN_SUN = 100;
public://zombies
	static const int ZOMBIE_EAT = 20;
};
#endif
/*		notes:
*		100 * real_time(in seconds) / UPDATE_TIME(in seconds) = game_time
* 
*		(this 100 shouldn't be change, it is the WAIT parameter in "game.h". It has something to do with 
*		mouse click, so changing it would lead to some problems.)
*/
