#include "bullet.h"
#include "zombie.h"
Bullet::Bullet(int xp, int yp, Game* gm, BackGround* bkg, int row,int mxx,int move_step,int fire)
	:Unit(xp, yp, gm, bkg,"pea"), MAX_X(mxx), move_step_(move_step),FIRE(fire),ATTACK_RANGE(move_step) {
	 row_ = row;
}
Bullet::Bullet(int xp, int yp, Game* gm, BackGround* bkg, int row,int mxx,const std::string& name,int move_step,int fire)
	:Unit(xp, yp, gm, bkg,name), MAX_X(mxx), move_step_(move_step),FIRE(fire),ATTACK_RANGE(move_step){
	 row_ = row;
}
void Bullet::Update() {
	if (x_ >= MAX_X)Removed();
	x_ += move_step_;
	// printf("pea at %d, row %d\n", x_, row_);
	Attack();
}
void Bullet::Attack() {
	auto info = bkg_->FindMinZombie(x_, row_);
	if (info.second == nullptr)return;
	if (info.first < x_) {
		dynamic_cast<Zombie*>(info.second)->DeduceLife(FIRE,name_);
		// printf("image status is now %d\n", 1);
		image_status_ = 1;
		Removed();
	}
}