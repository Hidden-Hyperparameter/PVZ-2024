#pragma once
#include "unit.h"
#include <stdio.h>
// #include <graphics.h>
#include <conio.h>
#include <time.h>
#include <cassert>
class Unit;
class Sun :public Unit {
public:
	const int move_step_;
	static const int SIZE = 10;
	const int MAX_Y;
public:
	//void Fall();
	Sun(int xp, int yp, int mv, Game* gm, BackGround* bkg, int mxnum);
	void Update() override;
};