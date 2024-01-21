#include "plant.h"
#include "game_parameters.h"
class Plant;
class CherryBomb:public Plant{
public:
bool explode_=false;
CherryBomb::CherryBomb(int xp, int yp, Game* gm, BackGround* bkg, int row=-1);
void Explode();
void Update()override;
};