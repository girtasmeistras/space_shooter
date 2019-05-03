#include "bullet.h"
#include <iostream>
#include "stdlib.h"

Bullet::Bullet(): Entity() {}

Bullet::Bullet(int x, int y, Direction dir) {

    alive = true;
    b_direction = dir;
    entity_pos.w = BULLET_WIDTH;
    entity_pos.h = BULLET_HEIGHT;
    entity_pos.x = x + entity_pos.w;
    entity_pos.y = y + entity_pos.h;

    if(b_direction == Direction::UP){
        entity_pos.y = y - 15;
        entity_pos.x += 3;//used so bullet shoots from center
        flag = Flag::FRIENDLY;
    }
    if(b_direction == Direction::DOWN){
        entity_pos.y = y + 30;
        entity_pos.x += entity_pos.w + 5;//enemy is twice as large
        flag = Flag::HOSTILE;
    }

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
        entity_pos.y += ENEMY_BULLET_SPEED;
	}


}

void Bullet::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect) {

}

void Bullets::load(const char* path, SDL_Renderer* w_ren){

    bullet_texture.load_texture(path, w_ren);

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

void Bullets::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect){

    for (auto it = bullets.begin(); it != bullets.end(); ++it)
	{
	    SDL_Rect temp = (*it)->get_pos();
		bullet_texture.draw(w_ren, d_rect, &temp);
	}
}

Player_Bullets::~Player_Bullets(){
    bullet_texture.destroy();
}

void Player_Bullets::handle_events(const SDL_Event & event, const SDL_Rect & p_pos){

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (event.type == SDL_KEYDOWN) {
        if (state[SDL_SCANCODE_SPACE] && event.key.repeat == 0) {
            Bullet* to_shoot = new Bullet(p_pos.x, p_pos.y, Direction::UP);
            bullets.push_back(to_shoot);
            Entity::entity_list.push_back(to_shoot);

        }

    }
}

void Enemy_Bullets::shoot(const SDL_Rect & e_pos){

    Bullet* to_shoot = new Bullet(e_pos.x, e_pos.y + e_pos.h, Direction::DOWN);
    bullets.push_back(to_shoot);
    Entity::entity_list.push_back(to_shoot);

}

void Enemy_Bullets::kill(){

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        (*it)->kill();
        it = bullets.erase(it);
        ++it;
    }
}
