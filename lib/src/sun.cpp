#include "sun.h"

Sun::Sun(int xp, int yp, int mv, Game* gm, BackGround* bkg)
	:Unit(xp, yp, gm, bkg, "sun"), move_step_(mv), MAX_Y(bkg->y_size_ - 100) {
	name_ = "sun"; 
}
void Sun::Update() {
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 1;
	if (y_ + move_step_>= MAX_Y)return;
	y_ += move_step_;//move
}