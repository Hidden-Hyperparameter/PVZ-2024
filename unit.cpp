#include "unit.h"
void Unit::Show() {
	std::cout << "put " << name_ << " at " << x_ << " ," << y_ << std::endl;
	Helper::PutTransparentImage(x_, y_, bkg_->GetImage(name_, image_status_));
}
void Unit::Update() {
	if (should_be_removed_)return;
	std::cout << "Update unit named " << name_ << std::endl;
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 0;
}
void Unit::Removed(){
	printf("Virtual\n");
	should_be_removed_ = true;
}
void Unit::MoveTo(int x, int y) {
	image_status_ = 0;
	x_ = x,y_ = y;
}