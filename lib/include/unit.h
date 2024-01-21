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
/**
 * @brief Construct a new Unit object without specifying name
 * 
 * @param xp x-pos
 * @param yp y-pos
 * @param gm game pointer
 * @param bkg background pointer
 */
	Unit(int xp, int yp, Game* gm, BackGround* bkg) ;
/**
 * @brief Construct a new Unit object given the name
 * 
 * @param xp x-pos
 * @param yp y-pos
 * @param gm game pointer
 * @param bkg background pointer
 * @param name unit name
 */
	Unit(int xp, int yp, Game* gm, BackGround* bkg,const std::string& name) ;
/**
 * @brief put image on screen
 * 
 */
	virtual void Show();
/**
 * @brief update status(like move), including the image status
 * 
 */
	virtual void Update();
/**
 * @brief to mark this unit object as removed
 * @attention after calling this function, the unit will still be put on screen ONCE.
 * 
 */
	virtual void Removed();
/**
 * @brief Change the postion to new one
 * 
 * @param x x-coord
 * @param y y-coord
 */
	void MoveTo(int x, int y);
};