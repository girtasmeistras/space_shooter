#include "bullet.h"
#include "helpers.h"
Bullet::Bullet() {


	b_pos.x = 0;
	b_pos.y = 0;


	b_pos.w = 7;
	b_pos.h = 7;


}

Bullet::~Bullet() {

	
}

void Bullet::update() {

	b_pos.y -= 15;


}



void Bullet::get_pos(int x, int y) {

	b_pos.x = x + b_pos.w;
	b_pos.y = y;

}