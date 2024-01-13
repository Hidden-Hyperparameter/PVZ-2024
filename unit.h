#pragma once
#include <stdio.h>
#include "background.h"
#include <string>
class BackGround;
class Game;
class Unit {
public:
	int x_, y_;
	BackGround* bkg_;
	std::string name_;
	Game* gm_;
	int image_status_=0;
	unsigned long long last_update_time_=0;
	//const unsigned long long WAIT;
	int max_image_num_;
	bool should_be_removed_ = false;
public:
	Unit(int xp, int yp, Game* gm, BackGround* bkg, int mxnum) :
		x_(xp), y_(yp), gm_(gm), bkg_(bkg) ,max_image_num_(mxnum){}
	virtual void Show();
	virtual void Update();
	virtual void Removed();
	void MoveTo(int x, int y);
};