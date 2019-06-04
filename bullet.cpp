#include "bullet.h"
#include <iostream>
#include "stdlib.h"

Bullet::Bullet(): Entity() {}

Bullet::Bullet(const SDL_Rect & start_pos, Direction p_dir, Direction s_dir, bool friendly, const Image & texture) {

    alive = true;
    p_b_direction = p_dir;
    s_b_direction = s_dir;
    Entity::load(texture);
    entity_pos.x = start_pos.x;
    entity_pos.y = start_pos.y;

    if(p_b_direction == Direction::UP){
        entity_pos.y -= entity_pos.h;
        entity_pos.x += start_pos.w/2 - entity_pos.w/2;
    }
    if(p_b_direction == Direction::DOWN){
        entity_pos.y += start_pos.h;
        entity_pos.x += start_pos.w/2 - entity_pos.w/2;
    }
    if(p_b_direction == Direction::LEFT){
        entity_pos.y += start_pos.h/2 - entity_pos.h/2;
        entity_pos.x -= entity_pos.w;
    }
    if(p_b_direction == Direction::RIGHT){
        entity_pos.y += start_pos.h/2 - entity_pos.h/2;
        entity_pos.x +=  start_pos.w;
    }
    if(friendly){
        flag = Flag::FRIENDLY;
    }
    else{
        flag = Flag::HOSTILE;
    }

}

Rocket::Rocket(const SDL_Rect & start_pos, Direction p_dir, Direction s_dir, bool friendly, const Image & texture):
    Bullet(start_pos, p_dir, s_dir, friendly, texture), counter(0)
{
    deleted_unique_ptr<Mix_Chunk> es (Mix_LoadWAV("sfx/swoosh.wav"), [](Mix_Chunk* c) { Mix_FreeChunk(c); });
    explosion_sound = std::move(es);
    assert(explosion_sound, Mix_GetError());
}

void Bullet::update() {

    Entity::update();

    if(p_b_direction == Direction::UP){
        if(entity_pos.y < 0){
            alive = false;
        }
        entity_pos.y -= BULLET_SPEED;

    }

	if(p_b_direction == Direction::DOWN && flag == Flag::FRIENDLY){
        if(entity_pos.y > SCREEN_HEIGHT){
            alive = false;
        }
        entity_pos.y += BULLET_SPEED;
	}

	if(p_b_direction == Direction::DOWN && flag == Flag::HOSTILE){
        if(entity_pos.y > SCREEN_HEIGHT){
            alive = false;
        }
        entity_pos.y += ENEMY_BULLET_SPEED;
	}

	if(p_b_direction == Direction::RIGHT){
        if(entity_pos.x > SCREEN_WIDTH){
            alive = false;
        }
        entity_pos.x += BULLET_SPEED;
    }

	if(p_b_direction == Direction::LEFT){
        if(entity_pos.x < 0){
            alive = false;
        }
        entity_pos.x -= BULLET_SPEED;
	}

}

void Acc_Bullet::update() {

    Entity::update();

    if(p_b_direction == Direction::UP){
        if(entity_pos.y < 0){
            alive = false;
        }
        entity_pos.y -= speed;

    }

	if(p_b_direction == Direction::DOWN && flag == Flag::FRIENDLY){
        if(entity_pos.y > SCREEN_HEIGHT){
            alive = false;
        }
        entity_pos.y += speed;
	}

	if(p_b_direction == Direction::DOWN && flag == Flag::HOSTILE){
        if(entity_pos.y > SCREEN_HEIGHT){
            alive = false;
        }
        entity_pos.y += speed;
	}

	if(p_b_direction == Direction::RIGHT){
        if(entity_pos.x > SCREEN_WIDTH){
            alive = false;
        }
        entity_pos.x += speed;
    }

	if(p_b_direction == Direction::LEFT){
        if(entity_pos.x < 0){
            alive = false;
        }
        entity_pos.x -= speed;
	}
    speed += ACCELERATION;
}

void Rocket::update(){

    Entity::update();
    counter++;
    if(counter < 30){
        entity_pos.y -= BULLET_SPEED/2;
    }
    else if(counter == 30){
        for(unsigned int i = 0;i < entity_list.size();i++) {
            if(this != entity_list[i].get() && entity_list[i].get() != nullptr && entity_list[i]->is_alive() == true && entity_list[i]->get_flag() == 1) {

                Entity_Collision collision;
                collision.first_entity = this;
                collision.second_entity = entity_list[i].get();
                Entity_Collision::entity_collision_list.push_back(collision);
            }
        }
        Mix_PlayChannel( -1, explosion_sound.get(), 0 );
    }
    else{
        playing_animation = true;
        alive = false;
    }

}

void Rocket::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

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

void Bullets::load(const char* path, SDL_Renderer* w_ren){

    bullet_texture.load_texture(path, w_ren);

}

void Bullets::load(const Image & texture){

    bullet_texture = texture;
}

void Player_Bullets::load_nuke(const Image & texture){

    nuke = texture;
}

void Player_Bullets::load_explosion(const Image & texture){

    explosion = texture;

    deleted_unique_ptr<Mix_Chunk> ns (Mix_LoadWAV("sfx/swoosh.wav"), [](Mix_Chunk* c) { Mix_FreeChunk(c); });
    nuke_sound = std::move(ns);
    assert(nuke_sound, Mix_GetError());

    deleted_unique_ptr<Mix_Chunk> bs (Mix_LoadWAV("sfx/bullet.wav"), [](Mix_Chunk* c) { Mix_FreeChunk(c); });
    bullet_sound = std::move(bs);
    assert(bullet_sound, Mix_GetError());
    //need to load these after mix has been initialised
}

bool is_dead(const std::shared_ptr<Bullet> & b){
    return !b->is_alive();
}


void update_bullet(std::shared_ptr<Bullet> & b){
    b->update();
}

void Bullets::update(){

    std::for_each(bullets.begin(), bullets.end(), update_bullet);
    //bullets.erase(std::remove_if(bullets.begin(), bullets.end(), is_dead), bullets.end());
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        if(!(*it)->is_alive()){
            it = bullets.erase(it);
        }else{
            ++it;
        }
    }
}


void Bullets::kill(){

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        (*it)->kill();
        it = bullets.erase(it);
    }

}

void Player_Bullets::handle_events(const SDL_Event & event, const SDL_Rect & p_pos, bool & has_rocket){

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_KEYDOWN) {
        if (state[SDL_SCANCODE_SPACE] && event.key.repeat == 0) {
            Mix_PlayChannel( -1, bullet_sound.get(), 0 );
            auto to_shoot = std::make_shared<Bullet>(p_pos, Direction::UP, Direction::NONE, true, bullet_texture);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(std::move(to_shoot));
        }
        if (state[SDL_SCANCODE_RETURN] && has_rocket && event.key.repeat == 0)
		{
		    Mix_PlayChannel( -1, nuke_sound.get(), 0 );
            auto to_shoot = std::make_shared<Rocket>(p_pos, Direction::UP, Direction::NONE, true, nuke);
            to_shoot->load_animation(explosion);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(std::move(to_shoot));
			has_rocket = false;
		}
		if (state[SDL_SCANCODE_W] && event.key.repeat == 0) {
            Mix_PlayChannel( -1, bullet_sound.get(), 0 );
            auto to_shoot = std::make_shared<Bullet>(p_pos, Direction::UP, Direction::NONE, true, bullet_texture);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(std::move(to_shoot));

        }
        if (state[SDL_SCANCODE_S] && event.key.repeat == 0) {
            Mix_PlayChannel( -1, bullet_sound.get(), 0 );
            auto to_shoot = std::make_shared<Bullet>(p_pos, Direction::DOWN, Direction::NONE, true, bullet_texture);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(std::move(to_shoot));

        }
        if (state[SDL_SCANCODE_D] && event.key.repeat == 0) {
            Mix_PlayChannel( -1, bullet_sound.get(), 0 );
            auto to_shoot = std::make_shared<Bullet>(p_pos, Direction::RIGHT, Direction::NONE, true, bullet_texture);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(std::move(to_shoot));

        }
        if (state[SDL_SCANCODE_A] && event.key.repeat == 0) {
            Mix_PlayChannel( -1, bullet_sound.get(), 0 );
            auto to_shoot = std::make_shared<Bullet>(p_pos, Direction::LEFT, Direction::NONE, true, bullet_texture);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(std::move(to_shoot));

        }
    }
}

void Enemy_Bullets::shoot(const SDL_Rect & e_pos, bool accelerate){

    if(!accelerate){
        auto to_shoot = std::make_shared<Bullet>(e_pos, Direction::DOWN, Direction::NONE, false, bullet_texture);
        bullets.push_back(to_shoot);
        Entity::entity_list.push_back(std::move(to_shoot));
    }
    else{
        auto to_shoot = std::make_shared<Acc_Bullet>(e_pos, Direction::DOWN, Direction::NONE, false, bullet_texture);
        bullets.push_back(to_shoot);
        Entity::entity_list.push_back(std::move(to_shoot));
    }
}


