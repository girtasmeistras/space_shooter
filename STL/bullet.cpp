#include "bullet.h"
#include "helpers.h"
projectile::projectile() {

	b_txt = nullptr;

	b_pos.x = 0;
	b_pos.y = 0;


	b_pos.w = 7;
	b_pos.h = 7;


}

projectile::~projectile() {

	SDL_DestroyTexture(b_txt);
}

void projectile::update() {

	b_pos.y -= 15;


}




void projectile::get_pos(int x, int y) {

	b_pos.x = x;
	b_pos.y = y;

}