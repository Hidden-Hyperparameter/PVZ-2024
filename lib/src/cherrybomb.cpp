#include "cherrybomb.h"
#include "zombie.h"
CherryBomb::CherryBomb(int xp, int yp, Game* gm, BackGround* bkg, int row):Plant(xp,yp,gm,bkg,"cherrybomb",row,Parameters::NO_DEATH_LIFE){
}
void CherryBomb::Update() {
	if (is_moving_)return;
	if (life_ < 0)return;
    if(!explode_)Explode();
	image_status_++;
	if (image_status_ == max_image_num_ - 1){
        image_status_ = 0;life_=-1;Removed();
}
}
void CherryBomb::Explode(){
    printf("the explode plant is at %d,%d\n",x_,y_);
    explode_=true;
    int col=(x_-bkg_->GRID_START_X)/bkg_->GRID_X;
    int min_col=col?(col-1):0;
    int min_row=row_?(row_-1):0;
    int max_col=(col==bkg_->GRID_X_NUM-1)?(col):(col+1);
    int max_row=(row_==bkg_->GRID_Y_NUM-1)?(row_):(row_+1);
    printf("row %d-%d,col %d-%d\n",min_row,max_row,min_col,max_col);
    auto lh=std::make_pair(min_col*bkg_->GRID_X+bkg_->GRID_START_X,min_row*bkg_->GRID_Y+bkg_->GRID_START_Y);
    auto rb=std::make_pair((max_col+1)*bkg_->GRID_X+bkg_->GRID_START_X,(max_row+1)*bkg_->GRID_Y+bkg_->GRID_START_Y);
    auto list=bkg_->GetZombieInRange(lh,rb);
    printf("find %d zombies\n",list.size());
    for(auto it:list){
        dynamic_cast<Zombie*>(it)->DeduceLife(0,"explode");
    }
}
