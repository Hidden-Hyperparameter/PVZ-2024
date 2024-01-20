#include "plant.h"
Plant::Plant(int xp, int yp, Game* gm, BackGround* bkg, int mxnum,int row,int max_life):
	Unit(xp, yp, gm, bkg, mxnum),max_life_(max_life),life_(max_life)
{
	type_ = "plant"; row_ = row;
}
void Plant::Update() {
	if (is_moving_)return;
	image_status_++;
	if (image_status_ == max_image_num_ - 1)image_status_ = 0;
	if (life_ < 0)return;
	Execute();
}
void Plant::Execute() {
	printf("virtual\n");
}
void Plant::DeduceLife(int amount) {
	life_ -= amount;
	if (life_ < 0) {
		Removed();
	}
}