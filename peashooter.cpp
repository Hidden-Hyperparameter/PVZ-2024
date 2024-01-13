#include "peashooter.h"

PeaShooter::PeaShooter(int xp, int yp, Game* gm, BackGround* bkg,  int mxnum) :
	Plant(xp, yp, gm, bkg, mxnum)
{
	name_ = "peashooter";
}
void PeaShooter::Execute() {
	bkg_->units_.insert(new Pea(x_, y_, gm_, bkg_, bkg_->GRID_END_X + 10));
}
