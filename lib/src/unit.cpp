#include "unit.h"
Unit::Unit(int xp, int yp, Game* gm, BackGround* bkg):
		x_(xp), y_(yp), gm_(gm), bkg_(bkg) {
		}
Unit::Unit(int xp, int yp, Game* gm, BackGround* bkg,const std::string& name):x_(xp), y_(yp), gm_(gm), bkg_(bkg) ,name_(name){
			max_image_num_=(gm_->GetImageNum(name_));
		}
void Unit::Show() {
	gm_->Draw(x_,y_,gm_->GetImage(name_,image_status_));
}
void Unit::Update() {
	if (should_be_removed_)return;
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 0;
}
void Unit::Removed(){
	should_be_removed_ = true;
}
void Unit::MoveTo(int x, int y) {
	image_status_ = 0;//during moving, the image(plants,specifically) shouldn't change
	x_ = x,y_ = y;
}