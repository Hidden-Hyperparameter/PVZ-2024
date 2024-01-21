#include "zombie.h"
#include "plant.h"
Zombie::Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, int row,int maxlife):
	Unit(xp, yp, gm, bkg,"zombie"), left_speed_(left_speed), max_life_(maxlife), life_(maxlife) {
	type_ = "zombie";
	row_ = row;
}
Zombie::Zombie(int xp, int yp,int left_speed, Game* gm, BackGround* bkg, const std::string& name,int row,int maxlife):
	Unit(xp, yp, gm, bkg, name), left_speed_(left_speed), max_life_(maxlife), life_(maxlife),MAX_EAT_IMAGE(gm_->GetImageNum(name+"/attack")) {
	type_ = "zombie";
	row_ = row;
}
void Zombie::Update() {
	if (life_ < 0)return;
	if (eating_plant_ != nullptr) {//continue eating
		eat_image_status++;
		if (eat_image_status == MAX_EAT_IMAGE - 1)eat_image_status = 0;
		Attack(); return;
	}
	//otherwise, use normal images
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 0;
	x_ -= left_speed_;
	//not eating, so find whether can eat a new plant
	auto info = bkg_->FindMaxPlant(x_, row_);//find plant to eat
	if (info.second && (info.first > x_-bkg_->GRID_X/2)) {//able to eat
		eating_plant_ = info.second;
	}
	if (x_ < bkg_->GRID_START_X-bkg_->GRID_X) {//reach house
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
void Zombie::DeduceLife(int amount,Parameters::Peatype type) {
	if (life_ < 0) {
		return;
	}
	life_ -= amount;
	if(life_<0){
		head_id_=gm_->MakeObject("zombie/head");
	}
}
void Zombie::Show() {
	if (life_ < 0) {//show dead images
		die_effect_curr_time++;
		bool do_update=false;
		if(die_effect_curr_time>=DIE_EFFECT_TIME){
			die_effect_curr_time=0;
			do_update=true;
		}
		bool head_show = false,body_show=false;
		if (die_head_status < gm_->GetImageNum(name_+"/head")-1){
			head_show=true,gm_->Draw(x_, y_, head_id_,"zombie/head", die_head_status);
			die_head_status+=do_update;

		}
		if (die_body_status < gm_->GetImageNum(name_+"/body")-1){
			body_show=true,gm_->Draw(x_, y_, id_,"zombie/body", die_head_status);
			die_body_status+=do_update;
		}
		if(!head_show&&!body_show) Removed();//remove after shown
	}
	else {
		if (eating_plant_ != nullptr) {//show eating images
			gm_->Draw(x_, y_, id_,name_ + "/attack", eat_image_status);
		}
		else {//show moving images
			gm_->Draw(x_, y_, id_,name_, image_status_);
		}
	}
}