#include "peashooter.h"

PeaShooter::PeaShooter(int xp, int yp, Game* gm, BackGround* bkg,  int row) :
	Plant(xp, yp, gm, bkg, "peashooter", row)
{
}
void PeaShooter::Execute() {
	pea_shoot_status++;
	if (pea_shoot_status == SHOOT_INTEVAL) {
		auto info = bkg_->FindMinZombie(x_, row_);
		if (/*info.second != nullptr*/true) {
			bkg_->units_.insert(new Pea(x_, y_, gm_, bkg_, row_, bkg_->GRID_END_X));
		}
		pea_shoot_status = 0;
	}
}
