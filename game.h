#pragma once
#include "unit.h"
#include "background.h"
#include <time.h>
#include <graphics.h>
#include <conio.h>
class BackGround;
class Game {
public:
	Game();
	void Show()const;
	void Init();
	void Update();
	unsigned long long GetTime()const { return time_; }
public:
	std::mt19937 gen;
	int x_size_ = 1000;
	int y_size_ = 1000;
	const unsigned long long WAIT = 100;
	BackGround* bkg_;
	unsigned long long time_=0;
	unsigned long long nxt_time_sun = 0;
	const int SUN_TIME = 5000;
public:
	int sun_cnt_ = 0;
};