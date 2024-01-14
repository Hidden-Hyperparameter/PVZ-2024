#include "background.h"
#include "helper.h"
#include "units.h"

void BackGround::LoadEveryThing() {
	//load images
//sun	
	sun_size_ = LoadUnit("sun");
	LoadUnit("pea");
	//plants
	LoadPlant("peashooter");
	LoadPlant("sunflower");
	//zombies
	LoadZombie("zombie");
}
std::pair<int,int> BackGround::LoadUnit(const char* s) {
	int i = 1;
	char path[64];
	std::vector<IMAGE*> images;
	while (i) {
		sprintf_s(path,sizeof(path), "assets/%s/%d.png", s, i);
		//std::cout << "path:" << (std::string)path << std::endl;
		IMAGE* im=new IMAGE;
		//char to wchar
		loadimage(im, Helper::CharToWchar(path));
		assert(im);
		i++;
		if (!im->getwidth())break;
		images.push_back(im);
	}
	assert(images.size());
	units_image_[(std::string)s] = images;
	units_max_image_num_[(std::string)s] = i-1;
	return std::make_pair(images[0]->getwidth(),images[0]->getheight());
}
std::pair<int,int> BackGround::LoadPlant(const char* s) {
	auto tmp = LoadUnit(s);
	bar_.push_back(s);
	//load card
	char path[64];
	sprintf_s(path, sizeof(path), "assets/cards/%s.png", s);
	IMAGE* im = new IMAGE;
	loadimage(im, Helper::CharToWchar(path));
	assert(im);
	Resize(im, CARD_X, CARD_Y);
	cards_image_[(std::string)s] = im;
	return tmp;
}
void BackGround::LoadZombie(const char* s) {
	std::string name = s;
	LoadUnit(name.c_str());
	std::string head_name = name + "/head";
	LoadUnit(head_name.c_str());
	std::string body_name = name + "/body";
	LoadUnit(body_name.c_str());
	std::string attack_name = name + "/attack";
	LoadUnit(attack_name.c_str());
}
BackGround::BackGround( Game* gm,int id) : gen_(time(NULL)),  gm_(gm),WAIT(gm->WAIT) {
	loadimage(&map_image_, _T("assets/map/map0.jpg"));
	loadimage(&chooser_image_, _T("assets/others/chooser.png"));
	assert(map_image_.getwidth());
	assert(chooser_image_.getwidth());
	LoadEveryThing();
	//set size back
	x_size_ = map_image_.getwidth();
	y_size_ = map_image_.getheight();
	gm_->x_size_ = x_size_; gm_->y_size_ = y_size_;
	InitPrice();
	grid_manager_.resize(GRID_X_NUM);
	for (int i = 0; i < GRID_X_NUM; i++) {
		grid_manager_[i].resize(GRID_Y_NUM, nullptr);
	}
}
void BackGround::InitPrice() {
	units_price_["peashooter"] = 100;
	units_price_["sunflower"] = 50;
}
void BackGround::Show(){
	//clearrectangle(0, 0, x_size_, y_size_);
	putimage(0, 0, &map_image_);
	putimage(CHOOSER_X, CHOOSER_Y, &chooser_image_);
	outtextxy(SUN_CNT_X, SUN_CNT_Y, Helper::NumToWchar(sun_cnt_));
	for (int i = 0; i < bar_.size();i++) {
		putimage(CARD_START_X + CARD_X * i, CARD_START_Y,
			cards_image_[bar_[i]]);
	}
	for (auto it = units_.begin(); it != units_.end();) {
		(*it)->Show();
		if ((*it)->should_be_removed_) {
			auto tmp = it;
			++it;
			units_.erase(tmp);
			continue;
		}
		++it;
	}
}
void BackGround::CreateSun(){
	auto xrd = std::uniform_int_distribution<int>(10, x_size_-sun_size_.first);
	auto yrd = std::uniform_int_distribution<int>(10, y_size_);
	units_.insert(new Sun(xrd(gen_), 100,y_size_/SUN_STEPS,gm_,this,units_max_image_num_["sun"]));
}
void BackGround::CreateZombie() {
	printf("make zombie\n");
	auto row = std::uniform_int_distribution<int>(0, GRID_Y_NUM-1);
	int rw = row(gen_);
	units_.insert(new Zombie(GRID_END_X,GRID_START_Y+(rw - 1) * GRID_Y,(GRID_END_X-GRID_START_X)/ZOMBIE_STEPS,
		gm_,this,units_max_image_num_["zombie"],rw));
}
void BackGround::UpdateUnits() {
	for (auto it : units_) {
		std::cout << "this unit is " << it->name_ << std::endl;
		it->Update();
	}
}
void BackGround::UserClick() {
	bool rst = peekmessage(&msg_,EX_MOUSE);
	if (!rst)return;
	if (msg_.message == WM_MOUSEMOVE) {
		if (placing_plant_.first) {
			std::cout << "move " << placing_plant_.second->name_ << "to " << msg_.x << " ," << msg_.y << std::endl;
			placing_plant_.second->MoveTo(msg_.x, msg_.y-GRID_Y/3);
		}
		return;
	}
	if (msg_.message == WM_LBUTTONDOWN) {
		printf("user click %d,%d\n", msg_.x, msg_.y);
		if ((msg_.x >= GRID_START_X) && (msg_.x <= GRID_END_X) && (msg_.y >= GRID_START_Y) && (msg_.y <= GRID_END_Y)) {
			if (placing_plant_.first) {
				printf("user click grid\n");
				int x_index = (msg_.x - GRID_START_X) / GRID_X;
				int y_index = (msg_.y - GRID_START_Y) / GRID_Y;
				if (grid_manager_[x_index][y_index] == nullptr) {
					placing_plant_.first = false;
					placing_plant_.second->is_moving_ = false;
					placing_plant_.second->MoveTo(GRID_START_X + GRID_X * x_index, GRID_START_Y + GRID_Y * y_index);
					placing_plant_.second->row_ = y_index;
					grid_manager_[x_index][y_index] = placing_plant_.second;
					return;
				}
				else {
					printf("place plant fail, already exist one\n");
					sun_cnt_ += units_price_[placing_plant_.second->name_];
					placing_plant_.first = false;
					units_.erase(placing_plant_.second);
				}
			}
		}
		if ((msg_.x >= CARD_START_X) && (msg_.x <= CARD_END_X) && (msg_.y >= CARD_START_Y) && (msg_.y <= CARD_Y)) {
			printf("user click chooser\n");
			int index = ((msg_.x-CARD_START_X) / CARD_X) ;
			if (index >= bar_.size())return;
			int price = units_price_[bar_[index]];
			if (sun_cnt_ >= price) {
				sun_cnt_ -= price;
				Plant* un = MakePlant(bar_[index], msg_.x, msg_.y);
				units_.insert(un);
				std::cout << "insert unit " << un->name_ << std::endl;
				placing_plant_ = std::make_pair(true, un);
			}
			return;
		}
		for (auto it : units_) {
			if (it->name_ != "sun")continue;
			if (abs(it->x_ - msg_.x) <= sun_size_.first && abs(it->y_ - msg_.y) <= sun_size_.second) {
				it->Removed();
				units_.erase(it);
				sun_cnt_ += 25;//single sun value, never changed
				printf("successful remove one\n");
				break;
			}
		}
		
	}
}

IMAGE* BackGround::GetImage(const std::string& s, int status){
	assert(units_image_[s].size() > status);
	return units_image_[s][status];
}
Plant* BackGround::MakePlant(std::string name, int x, int y) {
	std::cout << "making unit named " << name << std::endl;
	if (name == "peashooter")return new PeaShooter(
		x, y,gm_, this,  units_max_image_num_[name]
	);
	if (name == "sunflower")return new SunFlower(
		x, y, gm_, this, units_max_image_num_[name]
	);
	return new Plant(
		x, y, gm_, this,  units_max_image_num_[name]
	);
}
std::pair<int, Unit*> BackGround::FindMinZombie(int x, int row) {
	int min_x = x_size_; Unit* ret=nullptr;
	for (auto it : units_) {
		if (it->type_ == "zombie") {
			if (it->row_!=row)continue;
			if (it->x_ <min_x) {
				ret = it; min_x =it->x_;
			}
		}
	}
	printf("x-coordinate %d, first zombie behind is at %d\n", x, min_x);
	return std::make_pair(min_x, ret);
}
std::pair<int, Unit*> BackGround::FindMaxPlant(int x, int row) {
	int max_x = 0; Unit* ret = nullptr;
	for (auto it : units_) {
		if (it->type_ == "plant") {
			if (it->row_ != row)continue;
			if (it->x_ > max_x) {
				ret = it; max_x = it->x_;
			}
		}
	}
	printf("x-coordinate %d, first plant after is at %d\n", x, max_x);
	return std::make_pair(max_x, ret);
}