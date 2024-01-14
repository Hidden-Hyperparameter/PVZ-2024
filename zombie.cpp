#include "zombie.h"
#include "plant.h"
Zombie::Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int mxnum,int row,int maxlife):
	Unit(xp, yp, gm, bkg, mxnum), left_speed_(left_speed), max_life_(maxlife), life_(maxlife),MAX_EAT_IMAGE(bkg->units_max_image_num_["zombie/attack"]) {
	name_ = "zombie"; type_ = "zombie";
	row_ = row;
}
void Zombie::Update() {
	if (life_ < 0)return;
	if (eating_plant_ != nullptr) {
		eat_image_status++;
		if (eat_image_status == MAX_EAT_IMAGE - 1)eat_image_status = 0;
		Attack(); return;
	}
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 0;
	x_ -= left_speed_;
	//not eating, so find whether can eat a new plant
	auto info = bkg_->FindMaxPlant(x_, row_);
	if (info.second && (info.first > x_-bkg_->GRID_X/2)) {
		eating_plant_ = info.second;
		std::cout << "Eating " << eating_plant_->name_ << std::endl;
	}
	printf("zombie at %d,row %d\n", x_, row_);
	if (x_ < bkg_->GRID_START_X-bkg_->GRID_X) {
		gm_->Lose();
	}
}
void Zombie::Attack() {
	Plant* plt = dynamic_cast<Plant*>(eating_plant_);
	plt->DeduceLife(FIRE);
	if (plt->life_ < 0) {
		eating_plant_ = nullptr;
	}
}
void Zombie::DeduceLife(int amount) {
	printf("zombie deduce life\n");
	if (life_ < 0)return;
	life_ -= amount;
}
void Zombie::Show() {
	printf("zombie show\n");
	if (life_ < 0) {
		bool head_show = false,body_show=false;
		if (die_head_status < bkg_->units_max_image_num_["zombie/head"]-1)
			head_show=true,Helper::PutTransparentImage(x_, y_, bkg_->GetImage("zombie/head", die_head_status)),die_head_status++;
		if (die_body_status < bkg_->units_max_image_num_["zombie/body"]-1)
			body_show=true,Helper::PutTransparentImage(x_, y_, bkg_->GetImage("zombie/body", die_body_status)),die_body_status++;
		if(!head_show&&!body_show) Removed();
	}
	else {
		if (eating_plant_ != nullptr) {
			Helper::PutTransparentImage(x_, y_, bkg_->GetImage(name_ + "/attack", eat_image_status));
		}
		else {
			Helper::PutTransparentImage(x_, y_, bkg_->GetImage(name_, image_status_));
		}
	}
}