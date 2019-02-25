#pragma once
#include "SDL.h"

class projectile {

public:
	projectile();
	~projectile();

	void get_pos(int x, int y);
	void update();
	void draw(SDL_Renderer* window_renderer);
	void get_texture(SDL_Renderer* window_renderer);

	SDL_Texture* b_txt;
	SDL_Rect     b_pos;

};
