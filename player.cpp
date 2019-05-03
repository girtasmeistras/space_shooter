#include "player.h"
#include <iostream>
Player::Player() {

    entity_pos.x = SCREEN_WIDTH/2;
    entity_pos.y = SCREEN_HEIGHT/2;
    pp_dir = Direction::NONE;
    sp_dir = Direction::NONE;
    flag = Flag::FRIENDLY;
}

Player::~Player() {
    Entity::clean_up();
}

void Player::load_bullets(const char* path, SDL_Renderer* w_ren){

    p_bullets.load(path, w_ren);
}

int Player::get_health(){
    return health / 25;
}

void Player::restart(){
    alive = true;
    health = 100;
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

void Player::on_collision(Entity* entity){

    if( (int) flag != entity->get_flag()){
        health -= BULLET_DAMAGE;
    }

     if(health <= 0){
        alive = false;
        playing_animation = true;
    }
}

void Player::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect) {

    entity_texture.draw(w_ren, d_rect, &entity_pos);
    p_bullets.draw(w_ren, d_rect);
}

void Player::power_up() {
   health += 25;
}

void Player::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

    ++animation_counter;
    //int x = animation_counter % 4;
    //int y = animation_counter / 4;
    SDL_Rect clip;
    clip.h = EXPLOSION_CLIP;
    clip.w = EXPLOSION_CLIP;
    clip.x = (animation_counter % 4) * EXPLOSION_CLIP;
    clip.y = (animation_counter / 4) * EXPLOSION_CLIP;
    animation_texture.clipped_render(entity_pos.x, entity_pos.y, &clip, w_ren);

    if(animation_counter > 15){
        playing_animation = false;
        animation_counter = -1;
    }
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


