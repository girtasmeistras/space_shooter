#include "gameObject.h"
projectile::projectile() {

	m_image = SDL_LoadBMP("bullet.bmp");

	m_position.x = 0;
	m_position.y = 0;


	m_position.w = 10;
	m_position.h = 10;

	m_x = 0.0;
	m_y = 0.0;

}



void projectile::update(double delta_time) {

	m_x += (5.0 * delta_time);
	m_position.x = m_x;
}
