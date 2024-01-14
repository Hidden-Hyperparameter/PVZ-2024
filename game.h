#pragma once
#include "unit.h"
#include "background.h"
#include "game_parameters.h"
#include <time.h>
#include <graphics.h>
#include <conio.h>
class Parameters;
class BackGround;
class Game {
public:
	std::mt19937 gen;//random number generator
	BackGround* bkg_;
public:
	const unsigned long long WAIT = 100;
	const int SUN_TIME = Parameters::SUN_TIME;
	const int ZOMBIE_TIME = Parameters::ZOMBIE_TIME;
public:
	int x_size_ = 1000;//gameboard default size, will be set by BackGround later.
	int y_size_ = 1000;
	unsigned long long time_=0;//virtual time
	unsigned long long nxt_time_sun = 0;//next time generating sun
	unsigned long long nxt_time_zombie = Parameters::INIT_ZOMBIE_TIME;//next time generating zombie
	bool in_game_ = true;
public:

public:
	Game();
	void Show()const;
	void Init();//initalize the graphics interface
	void Update();//main loop
	void Lose();//show lose graphics effects
};