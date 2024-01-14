#include "sun.h"

Sun::Sun(int xp, int yp, int mv, Game* gm, BackGround* bkg, int mxnum)
	:Unit(xp, yp, gm, bkg, mxnum), move_step_(mv), MAX_Y(bkg->y_size_ - 100) {
	name_ = "sun"; type_ = "sun";
}
void Sun::Update() {
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 1;
	if (y_ + move_step_>= MAX_Y)return;
	//if (gm_->GetTime() - last_update_time_ < WAIT) return;
	y_ += move_step_;
	//last_update_time_ = gm_->GetTime();
}

void Sun::Show() {
	Helper::PutTransparentImage(x_, y_, bkg_->GetImage(name_, image_status_));
	//printf("print sun at %d,%d\n", x_, y_);
	//outtextxy(x_, y_, '*');
}

void Sun::Removed() {
	printf("removing sun at %d,%d\n", x_, y_);
	clearcircle(x_, y_, 10);
	//outtextxy(x_, y_, '*');
}
