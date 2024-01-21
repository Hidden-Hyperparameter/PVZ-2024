#pragma once
#include "unit.h"
#include "background.h"
#include "game_parameters.h"
#include <time.h>
#include <string>
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
	unsigned long long render_time_=0;//virtual time for render, not relavent in most scenes.
	unsigned long long nxt_time_sun = 0;//next time generating sun
	unsigned long long nxt_time_zombie = Parameters::INIT_ZOMBIE_TIME;//next time generating zombie
	bool in_game_ = true;
	/**
	 * @brief (unit_name,frame) ->image id
	 * @note images are unique.
	 */
	std::unordered_map<std::string,std::vector<Vulkan::App::image_t> > image_dict;
	/**
	 * @brief all the plants are available
	 * @note you don't have to modify them when you add a plant
	 * 
	 */
	std::vector<std::string> plant_available;
public:
	/**
	 * @brief Get how many images(i.e. frame) this name corosponds to
	 * 
	 * @param name the unit name
	 * @return const int number of images
	 */
	const int GetImageNum(const std::string& name);
	/**
	 * @brief Make an object and return the id
	 * 
	 * @param name the image name. If you are not sure, you can change this name to null at first. Note that setting the `name` variable is only an shortcut of creating an object and setting the image together(for example, in the case of loading the panel and chooser), instead of an requirement.
	 * @details If you set the name, then the Vulkan object is going to bind with image at begin. If you don't set it, then the Vulkan object is going to use the default image(empty.png).
	 * @return Vulkan::App::object_t: the object id in Vulkan
	 */
	Vulkan::App::object_t MakeObject(const std::string& name="");
public:
/**
 * @brief Construct a new Game object, also load all images
 * 
 */
	Game();
	/**
	 * @brief directly call Background::Show().
	 * 
	 */
	void Show()const;
	/**
	 * @brief directly call Background::ChoosePlants().
	 * 
	 */
	void Init();
	/**
	 * @brief This process contains: (1) user click (2) output image per specific intervals (3) update game objects(and call background create sun and zombies) per specific intervals
	 * 
	 */
	void Update();
	/**
	 * @brief show lose graphics effects, not implemented yet
	 * 
	 */
	void Lose();
	/**
	 * @brief load all images.
	 * @note Make sure that you add your new plants in the function
	 * 
	 */
	void LoadAll();
	/**
	 * @brief main function of game, contain the whole process after construct.
	 * 
	 */
	void Start();
	/**
	 * @brief draw an image with **left-top** position being (xp,yp)
	 * 
	 * @param xp x-pos
	 * @param yp y-pos
	 * @param id the object id
	 * @param image the image id
	 */
	void Draw(int xp,int yp,Vulkan::App::object_t id,Vulkan::App::image_t image);
	/**
	 * @brief draw an image with **left-top** position being (xp,yp)
	 * 
	 * @param xp x-pos
	 * @param yp y-pos
	 * @param id object id
	 * @param name image name
	 * @param frame image frame
	 */
	void Draw(int xp,int yp,Vulkan::App::object_t id,const std::string& name,int frame=0);
	/**
	 * @brief helper function, add "assets/" before the path
	 * 
	 * @param name original path
	 * @return std::string: the new path
	 */
	std::string ChangeName(const std::string &name) const ;
	/**
	 * @brief load the image given one name.
	 * @attention the image name must NOT begin with "assets/".
	 * @attention the image name MUST end with ".**" extension
	 * @param name the image name
	 * @return Vulkan::App::image_t: the image id in Vulkan
	 */
	Vulkan::App::image_t LoadOneImage(const std::string& name);
	/**
	 * @brief load the image given one name. this function contains an "assert" to make sure the image loading must succeed
	 * @attention the image name must NOT begin with "assets/".
	 * @attention the image name MUST end with ".**" extension
	 * @param name the image name
	 * @return Vulkan::App::image_t: the image id in Vulkan
	 */
	Vulkan::App::image_t LoadOneImageNoFailure(const std::string& name);
	/**
	 * @brief load multiple image for one object(such as plant) until one fail. This function also contains an assert such that the name must corosponds to at least one image.
	 * @note the name can be something like "zombie/eat" 
	 * 
	 * @param unit_name the unit name
	 */
	void LoadMultiple(const std::string& unit_name);
	/**
	 * @brief LoadPlant = LoadMultiple(name) + Load(cardname)
	 * 
	 * @param unit_name name of plant
	 */
	void LoadPlant(const std::string& unit_name);
	/**
	 * @brief LoadZombie loads all the needed actions(eat,die,explode) for zombies.
	 * 
	 * @param unit_name zombie name
	 */
	void LoadZombie(const std::string& unit_name);
	/**
	 * @brief helper function, Get the Image id
	 * 
	 * @param name the image name
	 * @param frame frame
	 * @return Vulkan::App::image_t (the image id)
	 */
	Vulkan::App::image_t GetImage(const std::string& name,int frame=0);
	/**
	 * @brief Get the Card Image object
	 * 
	 * @param name the name (NOT containing "_card" suffix)
	 * @return Vulkan::App::image_t card-image id
	 */
	Vulkan::App::image_t GetCardImage(const std::string& name);
	/**
	 * @brief similar as LoadImage, but return the image size
	 * 
	 * @param name the image name 
	 * @return std::pair<int,int> the (width,height) pair of image. if fail, return (0,0).
	 */
	std::pair<int,int> LoadAndGetImageSize(const std::string &name)const;
	/**
	 * @brief Get the Image Size when the image is already LOADED
	 * 
	 * @param name the image name
	 * @return std::pair<int,int> the image (width,height) pair 
	 */
	std::pair<int,int> GetImageSize(const std::string &name);
};