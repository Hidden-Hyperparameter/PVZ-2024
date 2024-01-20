#include "plant.h"
Plant::Plant(int xp, int yp, Game* gm, BackGround* bkg, int row,int max_life):
	Unit(xp, yp, gm, bkg),max_life_(max_life),life_(max_life)
{
	type_ = "plant"; row_ = row;
}
Plant::Plant(int xp, int yp, Game* gm, BackGround* bkg,const std::string& name, int row,int max_life):
	Unit(xp, yp, gm, bkg, name),max_life_(max_life),life_(max_life)
{
	type_ = "plant"; row_ = row;
	max_image_num_=gm_->GetImageNum(name_);
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