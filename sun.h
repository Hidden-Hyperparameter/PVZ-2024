#pragma once
#include "unit.h"
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <cassert>
class Sun :public Unit {
public:
	const int move_step_;
	static const int SIZE = 10;
	const int MAX_Y;
public:
	//void Fall();
	Sun(int xp, int yp, int mv, Game* gm, BackGround* bkg,int mxnum) :Unit(xp, yp, gm, bkg, mxnum), move_step_(mv),MAX_Y(bkg->y_size_ - 100) { name_ = "sun"; }
	void Show() override;
	void Update() override;
	void Removed() override;
};