#pragma once
#include "unit.h"
#include "background.h"
#include "game_parameters.h"
#include <time.h>
#include <string>
/**
 * FIXME:
 * these define lines should be 
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
	/**
	 * @brief (unit_name,frame) ->image id
	 * images are unique.
	 */
	std::unordered_map<std::string,std::vector<Vulkan::App::image_t> > image_dict;
	/**
	 * @brief (unit_name,frame) + unit_id ->image id
	 * images are not unique, use copy constructor
	 */
	// std::unordered_map<int,Vulkan::App::object_t>obj_dict;
	std::vector<std::string> plant_available;
public:
	const int GetImageNum(const std::string& name);
	Vulkan::App::object_t MakeObject(const std::string& name="");
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
	void Draw(int xp,int yp,Vulkan::App::object_t id,Vulkan::App::image_t image);
	void Draw(int xp,int yp,Vulkan::App::object_t id,const std::string& name,int frame=0);
	std::string ChangeName(const std::string &name) const ;
	int LoadOneImage(const std::string& name);
	int LoadOneImageNoFailure(const std::string& name);
	void LoadMultiple(const std::string& unit_name);
	void LoadPlant(const std::string& unit_name);
	void LoadZombie(const std::string& unit_name);
	Vulkan::App::image_t GetImage(const std::string& name,int frame=0);
	Vulkan::App::image_t GetCardImage(const std::string& name);
	std::pair<int,int> LoadAndGetImageSize(const std::string &name)const;
	std::pair<int,int> GetImageSize(const std::string &name);
};