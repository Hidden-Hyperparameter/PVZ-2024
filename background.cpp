#include "background.h"
#include "units.h"
#include "helper.h"
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
		if (!im->getwidth())break;
		images.push_back(im);
		i++;
	}
	assert(images.size());
	units_image_[(std::string)s] = images;
	units_max_image_num_[(std::string)s] = i;
	return std::make_pair(images[0]->getwidth(),images[0]->getheight());
}
std::pair<int,int> BackGround::LoadPlant(const char* s) {
	auto tmp = LoadUnit(s);
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
BackGround::BackGround( Game* gm,int id) : gen_(time(NULL)),  gm_(gm),WAIT(gm->WAIT) {
	loadimage(&map_image_, _T("assets/map/map0.jpg"));
	loadimage(&chooser_image_, _T("assets/others/chooser.png"));
	assert(map_image_.getwidth());
	assert(chooser_image_.getwidth());
	bar_ = { "peashooter" };
	//load images
//sun	
	sun_size_=LoadUnit("sun");
	LoadUnit("pea");
//plants
	//peashooter_size_ = 
	LoadPlant("peashooter");
// 
	//set size back
	x_size_ = map_image_.getwidth();
	y_size_ = map_image_.getheight();
	gm_->x_size_ = x_size_; gm_->y_size_ = y_size_;
	InitPrice();
}
void BackGround::InitPrice() {
	units_price_["peashooter"] = 0;
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
		if ((*it)->should_be_removed_) {
			auto tmp = it;
			++it;
			units_.erase(tmp);
			continue;
		}
		(*it)->Show(); ++it;
	}
}
void BackGround::CreateSun(){
	auto xrd = std::uniform_int_distribution<int>(10, x_size_-sun_size_.first);
	auto yrd = std::uniform_int_distribution<int>(10, y_size_);
	units_.insert(new Sun(xrd(gen_), 100,y_size_/SUN_STEPS,gm_,this,units_max_image_num_["sun"]));
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
			placing_plant_.second->MoveTo(msg_.x, msg_.y);
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
				placing_plant_.first = false;
				placing_plant_.second->is_moving_ = false;
				placing_plant_.second->MoveTo(GRID_START_X + GRID_X * x_index, GRID_START_Y + GRID_Y * y_index);
				return;
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
	return new Plant(
		x, y, gm_, this,  units_max_image_num_[name]
	);
}