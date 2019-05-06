#include "bullet.h"
#include <iostream>
#include "stdlib.h"

Bullet::Bullet(): Entity() {}

Bullet::Bullet(int x, int y, Direction dir, const Image & texture) {

    alive = true;
    b_direction = dir;
    Entity::load(texture);
    entity_pos.x = x;
    if(b_direction == Direction::UP){
        entity_pos.y = y - entity_pos.h;
        entity_pos.x += 22/2 - entity_pos.w/2;//centers bullet for player
        flag = Flag::FRIENDLY;
    }
    if(b_direction == Direction::DOWN){
        entity_pos.y = y + entity_pos.h;
        entity_pos.x += 30/2 - entity_pos.w/2;//centers bullet for enemy
        flag = Flag::HOSTILE;
    }

}

void Bullet::update() {

    Entity::update();

    if(b_direction == Direction::UP){
        if(entity_pos.y < 0){
            alive = false;
        }
        entity_pos.y -= BULLET_SPEED;
    }

	if(b_direction == Direction::DOWN){
        if(entity_pos.y > SCREEN_HEIGHT){
            alive = false;
        }
        entity_pos.y += ENEMY_BULLET_SPEED;
	}

}

void Bullets::load(const char* path, SDL_Renderer* w_ren){

    bullet_texture.load_texture(path, w_ren);

}

void Player_Bullets::load_nuke(const char* path, SDL_Renderer* w_ren){

    nuke.load_texture(path, w_ren);

}

void Bullets::load(const Image & texture){

    bullet_texture = texture;
}

void Bullets::update(){

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        (*it)->update();
        if ((*it)->is_alive() == false){
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
        ++it;
    }
}

void Player_Bullets::handle_events(const SDL_Event & event, const SDL_Rect & p_pos, bool & has_rocket){

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_KEYDOWN) {
        if (state[SDL_SCANCODE_SPACE] && event.key.repeat == 0) {
            Bullet* to_shoot = new Bullet(p_pos.x, p_pos.y, Direction::UP, bullet_texture);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(to_shoot);

        }
        if (state[SDL_SCANCODE_RETURN] && has_rocket && event.key.repeat == 0)
		{
            Rocket* to_shoot = new Rocket(p_pos.x, p_pos.y, Direction::UP, nuke);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(to_shoot);
			has_rocket = false;
		}
    }
}

void Enemy_Bullets::shoot(const SDL_Rect & e_pos){

    Bullet* to_shoot = new Bullet(e_pos.x, e_pos.y + e_pos.h, Direction::DOWN, bullet_texture);
    bullets.push_back(to_shoot);
    Entity::entity_list.push_back(to_shoot);
}


