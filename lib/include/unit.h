#pragma once
#include <stdio.h>
#include "background.h"
#include <string>
#include "helper.h"
class BackGround;
class Game;
class Unit {
public:
	int x_, y_;
	int row_ = -1;//start at 0
	std::string name_;
	std::string type_;
	bool should_be_removed_ = false;
	int image_status_ = 0;
	int max_image_num_;
	Vulkan::App::object_t id_;
public:
	BackGround* bkg_;
	Game* gm_;
public:
	Unit(int xp, int yp, Game* gm, BackGround* bkg) ;
	Unit(int xp, int yp, Game* gm, BackGround* bkg,const std::string& name) ;
		
	virtual void Show();//put image on screen
	virtual void Update();//update status(containing the image status)
	virtual void Removed();//note that this is removed
	void MoveTo(int x, int y);
};