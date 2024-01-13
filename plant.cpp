#include "plant.h"
Plant::Plant(int xp, int yp, Game* gm, BackGround* bkg, int mxnum):
	Unit(xp, yp, gm, bkg, mxnum)
{}
void Plant::Update() {
	if (is_moving_)return;
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 0;
	Execute();
}
void Plant::Execute() {
	printf("virtual\n");
}