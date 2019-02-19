#include "gameObject.h"
#include <iostream>
gameObject::gameObject()
{

	player_surface = SDL_LoadBMP("spaceship.bmp");
	

	m_position.x = 0;
	m_position.y = 0;

	m_position.w = 23;
	m_position.h = 22;

	m_x = 0.0;
	m_y = 0.0;

}

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



void gameObject::update(double delta_time)
{
	
		switch (m_direction)
		{
		case direction::NONE:
			m_x += 0.0;
			m_y += 0.0;
			break;
		case direction::UP:
			m_y = m_y - (20.0 * delta_time);
			break;
		case direction::DOWN:
			m_y = m_y + (20.0 * delta_time);
			break;
		case direction::LEFT:
			m_x = m_x - (20.0 * delta_time);
			break;
		case direction::RIGHT:
			m_x = m_x + (20.0 * delta_time);
			break;

		}
	


		switch (s_direction)
		{

		case direction::NONE:
			m_x += 0.0;
			m_y += 0.0;
			break;
		case direction::UP:
			m_y = m_y - (20.0 * delta_time);
			break;
		case direction::DOWN:
			m_y = m_y + (20.0 * delta_time);
			break;
		}




	m_position.x = m_x;
	m_position.y = m_y;

	

}

void gameObject::draw(SDL_Surface* window_surface)
{

	SDL_BlitSurface(player_surface, nullptr, window_surface, &m_position);
	
	
}

void gameObject::handleEvents(SDL_Event const &event) {

	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

	if (event.type == SDL_KEYDOWN) {
	

		if (keyboard_state_array[SDL_SCANCODE_UP])
		{
			m_direction = direction::UP;
			
		}
		if (keyboard_state_array[SDL_SCANCODE_DOWN])
		{
			m_direction = direction::DOWN;
			
		}

		if (keyboard_state_array[SDL_SCANCODE_RIGHT])
		{
			if (m_direction == direction::UP) {
				s_direction = direction::UP;
				
			}
			if (m_direction == direction::DOWN) {

				s_direction = direction::DOWN;
				
			}
			
				m_direction = direction::RIGHT;
				
			
		}
		if (keyboard_state_array[SDL_SCANCODE_LEFT])
		{
			if (m_direction == direction::UP) {

				s_direction = direction::UP;
			}

			if(m_direction == direction::DOWN) {
				
				s_direction = direction::DOWN;

			}
			
				m_direction = direction::LEFT;
				
		}

		if (keyboard_state_array[SDL_SCANCODE_SPACE]) {

			//shoot a projectile

		}
		

		
	}

	if (event.type == SDL_KEYUP) {

		if (s_direction == direction::NONE) {

			m_direction = direction::NONE;
		}
		else {

			if (s_direction == direction::UP && keyboard_state_array[SDL_SCANCODE_UP]) {

					m_direction = direction::UP;
				
			}
			if (s_direction == direction::DOWN && keyboard_state_array[SDL_SCANCODE_DOWN]) {
					
					m_direction = direction::DOWN;

			}


			s_direction = direction::NONE;
		}
		
		
		

	}


}