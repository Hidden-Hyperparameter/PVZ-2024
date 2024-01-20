#include "unit.h"
void Unit::Show() {
	Helper::PutTransparentImage(x_, y_, bkg_->GetImage(name_, image_status_));
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