#include "player.h"
#include <iostream>
player::player()
{

	p_pos.x = 0;
	p_pos.y = 0;
	p_pos.w = 23;
	p_pos.h = 22;
}

player::~player() {

	


}


void player::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect, img* b_txt)
{

	p_txt.draw(w_ren, d_rect, &p_pos);

	for (auto it = bullets.begin(); it != bullets.end(); ++it)
	{
		b_txt->draw(w_ren, d_rect, &(*it)->b_pos);
		

	}

}

void player::update()
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


		for (auto it = bullets.begin(); it != bullets.end();)
		{
			(*it)->update();

			if ((*it)->b_pos.y < 0)
			{
				delete (*it);
				bullets.erase(it++); 
				
			}

			else {
				++it;
			}
			

		}
		


	

}



void player::handleEvents(SDL_Event const &event) {

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

		if (state[SDL_SCANCODE_SPACE] && event.key.repeat == 0) {

			Bullet* to_shoot = new Bullet();
			to_shoot->get_pos(p_pos.x, p_pos.y);
			bullets.push_back(to_shoot);
			

		}
		

		
	}

	if (event.type == SDL_KEYUP) {

		if ((!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])) {

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

void player::get_texture(SDL_Renderer* w_ren) {

	p_txt.load_texture("spaceship.bmp", w_ren);

}