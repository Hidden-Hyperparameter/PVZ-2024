#pragma once
#include <unordered_set>
#include <random>
#include <graphics.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

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
	std::mt19937 gen_;
	std::unordered_set<Unit*> units_;
	std::vector<std::vector<Unit*>> grid_manager_;
	int x_size_, y_size_;
	const int SUN_STEPS =  Parameters::SUN_STEPS;
	const int ZOMBIE_STEPS =  Parameters::ZOMBIE_STEPS;
	const int CHOOSER_X = 100, CHOOSER_Y = 0;
	const int CARD_START_X = 175, CARD_START_Y = 7;
	const int CARD_END_X = 610;
	const int CARD_NUM = 8;
	const int CARD_X = 62, CARD_Y = 70;
	const int SUN_CNT_X = 130, SUN_CNT_Y = 65;
	const int GRID_START_X = 240, GRID_START_Y = 90;
	const int GRID_END_X = 980, GRID_END_Y = 570;
	const int GRID_X = 81, GRID_Y = 96;
	const int GRID_X_NUM = 10, GRID_Y_NUM = 5;
	ExMessage msg_;
	const int LOOP_FOR_MOUSE = 1;
	unsigned long long time_ = 0;
	const unsigned long long WAIT;
	Game* gm_;
	int map_id_=0;
	IMAGE map_image_;
	IMAGE chooser_image_;
	std::unordered_map<std::string, int> units_max_image_num_;
	std::unordered_map<std::string, std::vector<IMAGE*>> units_image_;
	std::unordered_map<std::string, IMAGE*> cards_image_;
	std::unordered_map<std::string, int> units_price_;
public:
	std::pair<int, int> sun_size_;
public:
	int sun_cnt_ = 50;
	std::vector<std::string> bar_;
	std::pair<bool, Plant*> placing_plant_;
public:
	BackGround( Game* gm,int id=0);
	std::pair<int,int> LoadUnit(const char* s);
	std::pair<int, int> LoadPlant(const char* s);
	void LoadZombie(const char* s);
	void Show();
	void CreateSun();
	void UpdateUnits();
	void UserClick();
	void InitPrice();
	IMAGE* GetImage(const std::string& s, int status=0);
	Plant* MakePlant(std::string name, int x, int y);
	void LoadEveryThing();
	void CreateZombie();
	std::pair<int,Unit*> FindMinZombie(int x, int row);//find zombie in given row having least x-coordinate
	std::pair<int, Unit*> FindMaxPlant(int x, int row);
};