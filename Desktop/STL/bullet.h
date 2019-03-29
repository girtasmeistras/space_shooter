#pragma once
#include "SDL.h"

class Bullet {

public:
	Bullet();
	Bullet(int x, int y);
	~Bullet();

	void get_pos(int x, int y);
	void update();

	SDL_Rect     b_pos;

};
