#pragma once
#include "unit.h"
#include "background.h"
#include "game_parameters.h"
#include <time.h>
#include <string>
/**
 * FIXME:
 * 
 *
 * 
 * 
*/
#define initgraph(a,b) 1
#define settextcolor(a)
#define setbkcolor(a)
#define setlinecolor(a)
#define GREEN 1
#define BLACK 1
#define clearrectangle(a,b,c,d) 1
// #include <graphics.h>
#include <conio.h>
#include "../../gui/lib/include/app.h"
class Parameters;
class BackGround;
class Game {
public:
	std::mt19937 gen;//random number generator
	BackGround* bkg_;
	Vulkan::App* app_;
public:
	const unsigned long long WAIT = 100;
	const int SUN_TIME = Parameters::SUN_TIME;
	const int ZOMBIE_TIME = Parameters::ZOMBIE_TIME;
public:
	int x_size_ = 1000;//gameboard default size, will be set by BackGround later.
	int y_size_ = 1000;
	unsigned long long time_=0;//virtual time
	unsigned long long render_time_=0;//virtual time
	unsigned long long nxt_time_sun = 0;//next time generating sun
	unsigned long long nxt_time_zombie = Parameters::INIT_ZOMBIE_TIME;//next time generating zombie
	bool in_game_ = true;
	std::unordered_map<std::string,std::vector<int> > image_dict;
	std::vector<std::string> plant_available;
public:
	const int GetImageNum(const std::string& name);
public:
	Game();
	void Show()const;
	void Init();//initalize the graphics interface
	void Update();//main loop
	void Lose();//show lose graphics effects
	void LoadAll();
	void Start();
	/**
	 * @brief draw an image with **left-top** position being (xp,yp)
	 * 
	 * @param xp 
	 * @param yp 
	 * @param image 
	 */
	void Draw(int xp,int yp,int image);
	std::string ChangeName(const std::string &name) const ;
	int LoadOneImage(const std::string& name);
	int LoadOneImageNoFailure(const std::string& name);
	void LoadMultiple(const std::string& unit_name);
	void LoadPlant(const std::string& unit_name);
	void LoadZombie(const std::string& unit_name);
	int GetImage(const std::string& name,int frame=0);
	int GetCardImage(const std::string& name);
	std::pair<int,int> GetImageSize(const std::string &name)const;
};