#include "peashooter.h"

PeaShooter::PeaShooter(int xp, int yp, Game* gm, BackGround* bkg, int mxnum, int row) :
	Plant(xp, yp, gm, bkg, mxnum, row){
	name_ = "peashooter";
	bullet_type_ = Parameters::Peatype::single;
}
void PeaShooter::Execute() {
	pea_shoot_status++;
	if (pea_shoot_status == SHOOT_INTEVAL) {
		auto info = bkg_->FindMinZombie(x_, row_);
		if (info.second != nullptr) {
			bkg_->units_.insert(new Pea(x_, y_, type_, gm_, bkg_, row_, bkg_->GRID_END_X));
		}
		pea_shoot_status = 0;
	}
}
