#include "game.h"
Game::Game() :bkg_(new BackGround(this)),gen(time(NULL)) {
	Init();
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
	printf("time %llu\n", time_);
	bkg_->UserClick();
	if (time_ % WAIT) {
		time_++; return;
	}
	if (time_ >= nxt_time_sun) {
		bkg_->CreateSun();
		std::uniform_int_distribution<unsigned long long> distr(0, SUN_TIME);
		nxt_time_sun = time_ + distr(gen);
	}
	bkg_->UpdateUnits();
	bkg_->Show();
	Sleep(500);
	time_++;
}
