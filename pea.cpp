#include "pea.h"
Pea::Pea(int xp, int yp, Game* gm, BackGround* bkg, int mxx)
	:Unit(xp, yp, gm, bkg, 1), MAX_X(mxx), move_step_(bkg->GRID_X) {
	name_ = "pea";
}
void Pea::Update() {
	if (x_ >= MAX_X)Removed();
	x_ += move_step_;
}