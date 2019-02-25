#include "gameObject.h"
#include <iostream>
gameObject::gameObject()
{
	p_txt = nullptr;
	bullet_is_present = false;

	p_pos.x = 0;
	p_pos.y = 0;
	p_pos.w = 23;
	p_pos.h = 22;
}




void gameObject::update()
{
	
		switch (pp_dir)
		{
		
		case direction::UP:
			
			p_pos.y = p_pos.y - 10;
			break;
		case direction::DOWN:
			
			p_pos.y = p_pos.y + 10;
			break;
		case direction::LEFT:
			
			p_pos.x = p_pos.x - 10;
			break;
		case direction::RIGHT:
			
			p_pos.x = p_pos.x + 10;

			break;

		}
	


		switch (sp_dir)
		{

		case direction::UP:
			
			p_pos.y = p_pos.y - 10;
			break;
		case direction::DOWN:
		
			p_pos.y = p_pos.y + 10;
			break;
		}

		if (p_pos.x > SCREEN_WIDTH - p_pos.w) {
			p_pos.x = SCREEN_WIDTH - p_pos.w;
		}

		if (p_pos.x < 0) {
			p_pos.x = 0;

		}

		if (p_pos.y > SCREEN_HEIGHT - p_pos.h) {
			p_pos.y = SCREEN_HEIGHT - p_pos.h;
		}
		if (p_pos.y < 0) {
			p_pos.y = 0;

		}


		if (bullet_is_present) {

			bullet.update();
		}


	

}

void gameObject::draw(SDL_Renderer* window_renderer, SDL_Rect* dest_rect)
{
	
	SDL_RenderCopy(window_renderer, p_txt, dest_rect, &p_pos);

	if (bullet_is_present) {

		bullet.draw(window_renderer);

		if (bullet.b_pos.y < 0) {

			bullet_is_present = false;

		}

	}

	
}

void gameObject::handleEvents(SDL_Event const &event) {

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (event.type == SDL_KEYDOWN) {
	

		if (state[SDL_SCANCODE_UP])
		{
			pp_dir = direction::UP;
			
		}
		if (state[SDL_SCANCODE_DOWN])
		{
			pp_dir = direction::DOWN;
			
		}

		if (state[SDL_SCANCODE_RIGHT])
		{
			if (pp_dir == direction::UP) {
				sp_dir = direction::UP;
				
			}
			if (pp_dir == direction::DOWN) {

				sp_dir = direction::DOWN;
				
			}
			
			pp_dir = direction::RIGHT;
				
			
		}
		if (state[SDL_SCANCODE_LEFT])
		{
			if (pp_dir == direction::UP) {

				sp_dir = direction::UP;
			}

			if(pp_dir == direction::DOWN) {
				
				sp_dir = direction::DOWN;

			}
			
			pp_dir = direction::LEFT;
				
		}

		if (state[SDL_SCANCODE_SPACE]) {

			bullet_is_present = true;
			bullet.get_pos(p_pos.x + bullet.b_pos.w, p_pos.y);

		}
		

		
	}

	if (event.type == SDL_KEYUP) {

		if (sp_dir == direction::NONE) {

			pp_dir = direction::NONE;
		}
		else {

			if (sp_dir == direction::UP &&  state[SDL_SCANCODE_UP]) {

				pp_dir = direction::UP;
				
			}
			if (sp_dir == direction::DOWN &&  state[SDL_SCANCODE_DOWN]) {
					
				pp_dir = direction::DOWN;

			}


			sp_dir = direction::NONE;
		}
		
		
		

	}


}

void gameObject::get_texture(SDL_Renderer* window_renderer) {

	p_txt = load_texture("spaceship.bmp", window_renderer);

}