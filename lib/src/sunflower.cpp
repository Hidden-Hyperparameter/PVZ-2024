#include "sunflower.h"
#include "sun.h"
SunFlower::SunFlower(int xp, int yp, Game* gm, BackGround* bkg, int mxnum, int row, int max_life):
	Plant(xp, yp, gm, bkg, mxnum, row, max_life),gen_sun_time_(GEN_SUN_INTERVAL/2) {
	name_ = "sunflower";
	bullet_type_ = Parameters::Peatype::none;
}
void SunFlower::Execute() {
	printf("sunflower gen sun time %d\n",gen_sun_time_);
	if (gen_sun_time_ == 0) {
		bkg_->units_.insert(new Sun(x_+bkg_->GRID_X, y_, 0, gm_, bkg_));
		std::uniform_int_distribution<int> next(GEN_SUN_INTERVAL / 2, GEN_SUN_INTERVAL);
		gen_sun_time_ = next(bkg_->gen_);
		printf("sunflower next gen sun time change to %d\n", gen_sun_time_);
	}
	else {g
		gen_sun_time_--;
	}
}