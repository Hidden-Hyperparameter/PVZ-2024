#include "game.h"
Game::Game() :bkg_(new BackGround(this)),gen(time(NULL)) {
	x_size_ = bkg_->x_size_, y_size_ = bkg_->y_size_;
}
void Game::Show()const {
	bkg_->Show();
}
void Game::Init() {
	initgraph(x_size_ + 10, y_size_ + 10);
	settextcolor(GREEN);
	setbkcolor(BLACK);
	setlinecolor(BLACK);
}
void Game::Update() {
	for (int i = 0; i < WAIT; i++) {
		bkg_->UserClick();
	}
	printf("time %llu\n", time_);
	if (time_ >= nxt_time_sun) {
		bkg_->CreateSun();
		std::uniform_int_distribution<unsigned long long> distr(0, SUN_TIME);
		nxt_time_sun = time_ + distr(gen);
	}
	if (time_ >= nxt_time_zombie) {
		bkg_->CreateZombie();
		std::uniform_int_distribution<unsigned long long> distr(0, ZOMBIE_TIME);
		nxt_time_zombie = time_ + distr(gen);
	}
	bkg_->UpdateUnits();
	bkg_->Show();
	Sleep(Parameters::UPDATE_TIME);
	time_++;
}
void Game::Lose() {
	in_game_ = false;
	clearrectangle(0, 0, x_size_, y_size_);
	outtextxy(x_size_ / 2, y_size_ / 2, __T("You Lose"));
	Sleep(5000);
}