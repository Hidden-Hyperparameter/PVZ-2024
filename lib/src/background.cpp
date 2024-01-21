#include "background.h"
#include "helper.h"
#include "units.h"
BackGround::BackGround( Game* gm,int id) : gen_(time(NULL)),  gm_(gm),map_id_(id) {
	printf("call background construction\n");
	
	//set size back
	
	InitPrice();
	grid_manager_.resize(GRID_X_NUM);
	for (int i = 0; i < GRID_X_NUM; i++) {
		grid_manager_[i].resize(GRID_Y_NUM, nullptr);
	}
}
void BackGround::ChoosePlants() {
	/*
	int x = 0, i = 0, mv = (x_size_ - gm_->x_size_) / STEPS;
	while (i < STEPS) {
		x += mv;
		Sleep(Parameters::UPDATE_TIME);
		putimage(0, 0, x_size_, y_size_, &map_image_, x, 0);
		i++;
	}
	int col = 0, row = 0;
	std::vector<std::vector<std::string> > panel_manager;
	std::vector<std::string> one_row;
	for (int i = 0; i < bar_.size(); i++) {
		one_row.push_back(bar_[i]);
		col++;
		if (col == CARD_NUM) {
			col = 0, row++;
			panel_manager.push_back(one_row);
			one_row.clear();
		}
	}
	panel_manager.push_back(one_row);
	//
	//FIXME:
	//this line is changed. See the commented line
	//
	int select_max = 0;
	// int select_max = std::min(bar_.size(), (size_t)CARD_NUM);
	bar_.clear();
	while (select_max) {
		putimage(0, 0, x_size_, y_size_, &map_image_, x, 0);
		gm_->Draw(CHOOSER_X,CHOOSER_Y,gm_->GetImage("chooser"));
		gm_->Draw(0,CARD_Y+CHOOSER_Y,gm_->GetImage("panel"));
		for (int i = 0; i < bar_.size(); i++) {
			putimage(CARD_START_X + CARD_X * i, CARD_START_Y,
				cards_image_[bar_[i]]);
		}
		for (int i = 0; i < panel_manager.size(); i++) {
			for (int j = 0; j < panel_manager[i].size(); j++) {
				auto it = panel_manager[i][j];
				if(it!="")gm_->Draw(PANEL_START_X + j * PANEL_X, 
					PANEL_START_Y + i * PANEL_Y,gm_->GetCardImage(it));
			}
		}
		for (int i = 0; i < gm_->WAIT; i++) {
			bool rst = peekmessage(&msg_, EX_MOUSE);//get user message
			if (!rst)continue;//no mouse actions
			if (msg_.message == WM_LBUTTONDOWN) {//user click
				if ((msg_.x >= PANEL_START_X) && (msg_.x <= PANEL_END_X) && (msg_.y >= PANEL_START_Y) && (msg_.y <= PANEL_END_Y)) {
					int x_index = (msg_.x - PANEL_START_X) / PANEL_X;
					int y_index = (msg_.y - PANEL_START_Y) / PANEL_Y;
					auto& it = panel_manager[y_index][x_index];
					if (it != "") {
						bar_.push_back(it);
						it = "";
						select_max--;
						break;
					}
				}
			}
		}
		if (select_max == 0)break;
		Sleep(Parameters::UPDATE_TIME);
	}
	x_size_ = gm_->x_size_;//reset x size, so that zombies will appear at real place

	*/
}

void BackGround::InitPrice() {
	units_price_["peashooter"] = 0;
	units_price_["sunflower"] = 0;
}
int BackGround::UnitCnt(){return unit_cnt_++;}
void BackGround::Show(){
	gm_->Draw(0, 0, gm_->GetImage("map"));
	gm_->Draw(CHOOSER_X, CHOOSER_Y, gm_->GetImage("chooser"));
	// outtextxy(SUN_CNT_X, SUN_CNT_Y, Helper::NumToWchar(sun_cnt_));//show sun numbers
	for (int i = 0; i < bar_.size();i++) {
		gm_->Draw(CARD_START_X + CARD_X * i, CARD_START_Y,gm_->GetCardImage(bar_[i]));
	}
	//show units as well as erasing the garbage(died) units.
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
	units_.insert(new Sun(xrd(gen_), 100,y_size_/SUN_STEPS,gm_,this));
}
void BackGround::CreateZombie() {
	auto row = std::uniform_int_distribution<int>(0, GRID_Y_NUM-1);
	int rw = row(gen_);
	units_.insert(new Zombie(GRID_END_X,GRID_START_Y+(rw - 1) * GRID_Y,(GRID_END_X-GRID_START_X)/ZOMBIE_STEPS,
		gm_,this,rw));
}
void BackGround::UpdateUnits() {
	for (auto it : units_) {
		it->Update();
	}
}
void BackGround::UserClick() {
	bool press;
	int xp,yp;
	std::tie(press,std::tie(xp,yp)) = gm_->app_->Mouse();
	if (press) {//user click
		printf("user click %d,%d\n", xp,yp);
		if ((xp >= GRID_START_X) && (xp <= GRID_END_X) && (yp>= GRID_START_Y) && (yp <= GRID_END_Y)) {
			if (placing_plant_.first) {//place the plant onto grid
				printf("user click grid\n");
				int x_index = (xp - GRID_START_X) / GRID_X;
				int y_index = (yp - GRID_START_Y) / GRID_Y;
				if (grid_manager_[x_index][y_index] == nullptr) {//no other plants exist on the grid
					placing_plant_.first = false;//finish place
					placing_plant_.second->is_moving_ = false;
					//set information of the plant
					placing_plant_.second->MoveTo(GRID_START_X + GRID_X * x_index, GRID_START_Y + GRID_Y * y_index);
					placing_plant_.second->row_ = y_index;
					grid_manager_[x_index][y_index] = placing_plant_.second;
					return;
				}
				else {
					printf("place plant fail, already exist one\n");
					sun_cnt_ += units_price_[placing_plant_.second->name_];//change sun back
					placing_plant_.first = false;
					units_.erase(placing_plant_.second);//this plant is used
				}
			}
		}
		if ((xp >= CARD_START_X) && (xp <= CARD_END_X) && (yp >= CARD_START_Y) && (yp <= CARD_Y)) {//select plant
			int index = ((xp-CARD_START_X) / CARD_X) ;
			printf("user click chooser index %d\n",index);
			if (index >= bar_.size())return;//click on empty bar
			int price = units_price_[bar_[index]];
			if (sun_cnt_ >= price) {
				sun_cnt_ -= price;
				Plant* un = MakePlant(bar_[index], xp, yp);//make a new plant
				units_.insert(un);
				placing_plant_ = std::make_pair(true, un);//let it follow mouse in future iterations
			}
			return;
		}
		for (auto it : units_) {
			if (it->name_ != "sun")continue;
			if (abs(it->x_ - xp) <= sun_size_.first && abs(it->y_ - yp) <= sun_size_.second) {//collect sun 
				it->Removed();
				units_.erase(it);
				sun_cnt_ += 25;//single sun value, never changed
				break;
			}
		}
	}else {//user move mouse
		if (placing_plant_.first) {//the recently-bought plant is following the mouse.
			printf("plant move to %d,%d\n",xp,yp-GRID_Y/3);
			placing_plant_.second->MoveTo(xp, yp-GRID_Y/3);
		}
	}
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