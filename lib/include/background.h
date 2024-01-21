#pragma once
#include <unordered_set>
#include <random>
// #include <graphics.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

#include "../../gui/lib/include/app.h"
#include "unit.h"
#include "game.h"
#include "game_parameters.h"
class Game;
class Unit;
class Plant;
class Parameters;
class Zombie;
class BackGround {
public:
	Game* gm_;
	int map_id_ = 0;
public:
	std::mt19937 gen_;
	std::unordered_set<Unit*> units_;
	std::vector<std::vector<Unit*>> grid_manager_;
public:
	int x_size_, y_size_;
	const int SUN_STEPS =  Parameters::SUN_STEPS;
	const int ZOMBIE_STEPS =  Parameters::ZOMBIE_STEPS;
	const int STEPS = 2;//inital choose plant steps
public:
	/**
	 * @brief graphics constants(unit: pixels)
	 * @note note that the y axis point from top to bottom
	 * 
	 */
	const int CHOOSER_X = 100, CHOOSER_Y = 0;
	const int CARD_START_X = 130, CARD_START_Y = 7;
	const int CARD_END_X = 610;
	const int CARD_NUM = 8;
	const int CARD_X = 54, CARD_Y = 70;
	const int SUN_CNT_X = 130, SUN_CNT_Y = 65;//the place for showing current sun counts
	const int GRID_START_X = 240, GRID_START_Y = 90;//left-top corner
	const int GRID_END_X = 980, GRID_END_Y = 570;//right-bottom corner
	const int GRID_X = 81, GRID_Y = 96;//single grid (average) size
	const int GRID_X_NUM = 10, GRID_Y_NUM = 5;
	const int PANEL_START_X = 24, PANEL_START_Y = 111;
	const int PANEL_X = 53, PANEL_Y = 74;
	const int PANEL_END_X = 439, PANEL_END_Y = 469;
public:
	Vulkan::App::object_t map_obj_id_;
	Vulkan::App::object_t chooser_obj_id_;  
	Vulkan::App::object_t panel_obj_id_;
	std::unordered_map<std::string, int> units_price_;//price(unit:suns)
public:
	std::pair<int, int> sun_size_;//this is used for collecting sun
public:
	int sun_cnt_ = 50;//initial sun count is 50
	/**
	 * @brief the number of units constructed.
	 * 
	 */
	int unit_cnt_=0;
	std::vector<std::pair<std::string,Vulkan::App::object_t> > bar_;//bar_=chooser, the place to click plants and plant them
	std::pair<bool, Plant*> placing_plant_=std::make_pair(false,nullptr);//the plant which is currently bought and following the mouse. If .first is false, then no such plant exist
public:
/**
 * @brief Construct a new Background object
 * 
 * @param gm Game
 * @param id the id of map
 */
	BackGround( Game* gm,int id=0);
/**
 * @brief set the plant prices
 * @attention modify this line when you add a new plant.
 */
	void InitPrice();
/**
 * @brief This function show the initial stage fo choosing plants. It haven't been implemented yet
 * 
 */
	void ChoosePlants();
/**
 * @brief get the new unit id and add the total unit count by 1
 * 
 * @return Vulkan::App::object_t(the object id)
 */
	Vulkan::App::object_t UnitCnt();
	/**
	 * @brief Create a Sun.
	 * 
	 */
	void CreateSun();
	/**
	 * @brief Create a Zombie.
	 * 
	 */
	void CreateZombie();
	/**
	 * @brief update the units to let them call the "Update" function
	 * 
	 */
	void UpdateUnits();
	/**
	 * @brief interact with the user. (1) click grid and place plant (2) click chooser and create plant (3) click sun (4) the plant "in flight" follow the mouse
	 * 
	 */
	void UserClick();
	/**
	 * @brief show the units(call "Show"), as well as the background and chooser.
	 * 
	 */
	void Show();
/**
 * @brief make a new plant
 * @attention This function must be modified when you add a new plant type
 * @param name name of plant, meet the folder name
 * @param x postions, @param y positions
 * @return Plant* the plant created
 */
	Plant* MakePlant(std::string name, int x, int y);
	/**
	 * @brief find the zombie with minimum pos satisfying pos.x>x given that the zombie is in one specific row
	 * 
	 * @param x the minimum pos
	 * @param row the row given
	 * @return std::pair<int,Unit*>: the first value is the x-coord of zombie, and the second is the pointer.
	 */
	std::pair<int,Unit*> FindMinZombie(int x, int row);
	/**
	 * @brief find the plant with maximum pos satisfying pos.x<x given that the plant is in one specific row
	 * 
	 * @param x the maximum pos
	 * @param row the row given
	 * @return std::pair<int,Unit*>: the first value is the x-coord of plant, and the second is the pointer.
	 */
	std::pair<int, Unit*> FindMaxPlant(int x, int row);
	/**
	 * @brief Get all zombies in the range left-top ~ right-bottom, note that the units are in pixels. Feel free to modify it if you think using rows and columns are more convenient.
	 * 
	 * @param left_top the left-top corner of the triangle range
	 * @param right_bot the right-bottom corner of the triangle range
	 * @return std::vector<Unit*> all zombies found
	 */
	std::vector<Unit*> GetZombieInRange(std::pair<int,int> left_top,std::pair<int,int> right_bot);
};