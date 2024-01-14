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
	Game();
	void Show()const;
	void Init();
	void Update();
	unsigned long long GetTime()const { return time_; }
	void Lose();
public:
	std::mt19937 gen;
	int x_size_ = 1000;
	int y_size_ = 1000;
	const unsigned long long WAIT = 100;
	BackGround* bkg_;
	unsigned long long time_=0;
	unsigned long long nxt_time_sun = 0;
	unsigned long long nxt_time_zombie = Parameters::INIT_ZOMBIE_TIME;
	const int SUN_TIME = Parameters::SUN_TIME;
	const int ZOMBIE_TIME = Parameters::ZOMBIE_TIME;
	bool in_game_ = true;
public:
	int sun_cnt_ = 0;
};