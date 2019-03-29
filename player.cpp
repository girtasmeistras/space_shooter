#include "player.h"
#include <iostream>
Player::Player() {

    entity_pos.x = SCREEN_WIDTH/2;
    entity_pos.y = SCREEN_HEIGHT/2;
    pp_dir = Direction::NONE;
    sp_dir = Direction::NONE;
    flag = Flag::PLAYER;
}

Player::~Player() {}

void Player::load_bullets(const char* path, SDL_Renderer* w_ren){

    p_bullets.load(path, w_ren);
}

int Player::get_health(){
    return health;
}

void Player::update()
{
    Entity::update();
    p_bullets.update();

		switch (pp_dir)
		{

		case Direction::UP:

			entity_pos.y = entity_pos.y - SPEED;
			break;
		case Direction::DOWN:

			entity_pos.y = entity_pos.y + SPEED;
			break;
		case Direction::LEFT:

			entity_pos.x = entity_pos.x - SPEED;
			break;
		case Direction::RIGHT:

			entity_pos.x = entity_pos.x + SPEED;
			break;
        default:
            break;

		}



		switch (sp_dir)
		{

		case Direction::UP:
			entity_pos.y = entity_pos.y - SPEED;
			break;

		case Direction::DOWN:
			entity_pos.y = entity_pos.y + SPEED;
			break;

        default:
            break;
		}

		if (entity_pos.x > SCREEN_WIDTH - entity_pos.w) {
			entity_pos.x = SCREEN_WIDTH - entity_pos.w;
		}

		if (entity_pos.x < 0) {
			entity_pos.x = 0;

		}

		if (entity_pos.y > SCREEN_HEIGHT - entity_pos.h) {
			entity_pos.y = SCREEN_HEIGHT - entity_pos.h;
		}
		if (entity_pos.y < 0) {
			entity_pos.y = 0;

		}




}

void Player::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect) {


    entity_texture.draw(w_ren, d_rect, &entity_pos);
    p_bullets.draw(w_ren, d_rect);

}

void Player::handle_events(const SDL_Event & event) {

    p_bullets.handle_events(event, entity_pos);

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (event.type == SDL_KEYDOWN) {


		if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN])
		{
			pp_dir = Direction::UP;

		}
		if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP])
		{
			pp_dir = Direction::DOWN;

		}

		if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT])
		{
			if (pp_dir == Direction::UP) {
				sp_dir = Direction::UP;

			}
			if (pp_dir == Direction::DOWN) {

				sp_dir = Direction::DOWN;

			}

			pp_dir = Direction::RIGHT;


		}
		if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])
		{
			if (pp_dir == Direction::UP) {

				sp_dir = Direction::UP;
			}

			if(pp_dir == Direction::DOWN) {

				sp_dir = Direction::DOWN;

			}

			pp_dir = Direction::LEFT;

		}


	}

	if (event.type == SDL_KEYUP) {

		if (sp_dir == Direction::NONE &&(!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])) {

			pp_dir = Direction::NONE;

		}

		else {

			if (sp_dir == Direction::UP &&  state[SDL_SCANCODE_UP]) {

				pp_dir = Direction::UP;

			}
			if (sp_dir == Direction::DOWN &&  state[SDL_SCANCODE_DOWN]) {

				pp_dir = Direction::DOWN;

			}


			sp_dir = Direction::NONE;
		}


	}


}


