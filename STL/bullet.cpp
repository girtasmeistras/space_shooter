#include "bullet.h"
#include "helpers.h"
projectile::projectile() {

	b_txt = nullptr;

	b_pos.x = 0;
	b_pos.y = 0;


	b_pos.w = 10;
	b_pos.h = 10;


}



void projectile::update() {

	b_pos.y -= 15;


}

void projectile::get_texture(SDL_Renderer* window_renderer) {

	b_txt = load_texture("bullet.bmp", window_renderer);

}

void projectile::draw(SDL_Renderer* window_renderer) {

	SDL_RenderCopy(window_renderer, b_txt, nullptr, &b_pos);

}


void projectile::get_pos(int x, int y) {

	b_pos.x = x;
	b_pos.y = y;

}