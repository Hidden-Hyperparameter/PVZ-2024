#include "sunflower.h"
#include "sun.h"
SunFlower::SunFlower(int xp, int yp, Game* gm, BackGround* bkg, int mxnum, int row, int max_life):
	Plant(xp, yp, gm, bkg, "sunflower", row, max_life),gen_sun_time_(GEN_SUN_INTERVAL/2) {
}
void SunFlower::Execute() {
	if (gen_sun_time_ == 0) {
		bkg_->units_.insert(new Sun(x_+bkg_->GRID_X, y_, 0, gm_, bkg_));
		std::uniform_int_distribution<int> next(GEN_SUN_INTERVAL / 2, GEN_SUN_INTERVAL);
		gen_sun_time_ = next(bkg_->gen_);
		printf("sunflower gen sun time change to %d\n", gen_sun_time_);
	}
	else {
		gen_sun_time_--;
	}
}