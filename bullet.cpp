#include "bullet.h"
#include <iostream>


Bullet::Bullet(): Entity() {}


Bullet::Bullet(int x, int y, Direction dir) {
    alive = true;
    b_direction = dir;
    entity_pos.w = 7;
    entity_pos.h = 7;
    entity_pos.x = x + entity_pos.w;

    if(b_direction == Direction::UP){
        entity_pos.y = y - 20;
        hurts_player = false;
    }
    if(b_direction == Direction::DOWN){
        entity_pos.y = y;
        entity_pos.x += entity_pos.w;//enemy is twice as large
        hurts_player = true;
    }

    flag = Flag::BULLET;


}

Bullet::~Bullet(){}

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
        entity_pos.y += BULLET_SPEED;

	}

}

void Bullets::load(const char* path, SDL_Renderer* w_ren){

    bullet_texture.load_texture(path, w_ren);

}

void Bullets::load(Image & texture){

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

void Bullets::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect){

    for (auto it = bullets.begin(); it != bullets.end(); ++it)
	{
	    SDL_Rect temp = (*it)->get_pos();
		bullet_texture.draw(w_ren, d_rect, &temp);

	}

}



void Player_Bullets::handle_events(const SDL_Event & event, SDL_Rect p_pos){

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_KEYDOWN) {
        if (state[SDL_SCANCODE_SPACE] && event.key.repeat == 0) {

            Bullet* to_shoot = new Bullet(p_pos.x, p_pos.y, Direction::UP);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(to_shoot);

        }

    }

}

void Enemy_Bullets::shoot(SDL_Rect e_pos){

    Bullet* to_shoot = new Bullet(e_pos.x, e_pos.y + e_pos.h, Direction::DOWN);
    bullets.push_back(to_shoot);
    Entity::entity_list.push_back(to_shoot);

}
