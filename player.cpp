#include "player.h"
#include "application.h"
#include <iostream>
Player::Player() {

    pp_dir = Direction::NONE;
    sp_dir = Direction::NONE;
    flag = Flag::FRIENDLY;
    deleted_unique_ptr<Mix_Chunk> hs (Mix_LoadWAV("sfx/hint.wav"), [](Mix_Chunk* c) { Mix_FreeChunk(c); });
    health_sound = std::move(hs);
    assert(health_sound, Mix_GetError());
}

Player::~Player() {

}

void Player::load_bullets(const Image & texture){

    p_bullets.load(texture);
}

void Player::load_nuke(const Image & texture){

    p_bullets.load_nuke(texture);
    p_bullets.load_explosion(animation_texture);
}

int Player::get_health() const{
    return health / 25;
}

void Player::restart(){

    alive = true;
    health = 100;
    has_rocket = false;
    entity_pos.x = SCREEN_WIDTH/2 - entity_pos.w/2;
    entity_pos.y = SCREEN_HEIGHT/2 + 60;
    movement_pos.set_x(entity_pos.x);
    movement_pos.set_y(entity_pos.y);
    pp_dir = Direction::NONE;
    sp_dir = Direction::NONE;
    p_bullets.kill();
}

void Player::update()
{
    Entity::update();
    p_bullets.update();
    switch (pp_dir)
		{
		case Direction::UP:
			movement_pos >> SPEED * DELTA_TIME;
			break;
		case Direction::DOWN:
			movement_pos > SPEED * DELTA_TIME;
			break;
		case Direction::LEFT:
			movement_pos << SPEED * DELTA_TIME;
			break;
		case Direction::RIGHT:
			movement_pos < SPEED * DELTA_TIME;
			break;
        default:
            break;
		}
		switch (sp_dir)
		{
		case Direction::UP:
			movement_pos >> SPEED * DELTA_TIME;
			break;
		case Direction::DOWN:
			movement_pos > SPEED * DELTA_TIME;
			break;
        default:
            break;
		}

        if (movement_pos.get_x() > SCREEN_WIDTH - entity_pos.w) {
			movement_pos.set_x(0);
		}
		if (movement_pos.get_x() < 0) {
			movement_pos.set_x(SCREEN_WIDTH-entity_pos.w);
		}
		if (movement_pos.get_y() > SCREEN_HEIGHT - entity_pos.h) {
			movement_pos.set_y(0);
		}
		if (movement_pos.get_y() < 0) {
			movement_pos.set_y(SCREEN_HEIGHT-entity_pos.h);
		}
		entity_pos.x = movement_pos.get_x();
		entity_pos.y = movement_pos.get_y();
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

void Player::power_up(int type) {
    Mix_PlayChannel( -1, health_sound.get(), 0 );
    if(type == 0){
        if(health < 100){
            health += 25;
        }
    }
    if(type == 1){
        has_rocket = true;
    }
}

void Player::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

    ++animation_counter;
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

    p_bullets.handle_events(event, entity_pos, has_rocket);
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


