#include "enemy.h"
#include <iostream>
#include "stdlib.h"

Enemy::Enemy(): Entity(), counter(0) {
    flag = Flag::ENEMY;

}

Enemy::Enemy(Image & enemy_texture, Image & enemy_bullet_texture, Image & enemy_explosion): Entity(), counter(0) {

    flag = Flag::ENEMY;
    load(enemy_texture);
    load_animation(enemy_explosion);
    load_bullets(enemy_bullet_texture);
    entity_pos.x = rand() % SCREEN_WIDTH;

}

Enemy::~Enemy() {}

void Enemies::create_new_enemy(){

    Enemy* new_enemy = new Enemy(enemy_texture, enemy_bullet_texture, enemy_explosion_texture);
    enemies.push_back(new_enemy);
    Entity::entity_list.push_back(new_enemy);
}

void Enemy::load_bullets(Image & bullet_texture){

    e_bullets.load(bullet_texture);
}

void Enemies::load_bullets(const char* path, SDL_Renderer* w_ren){

    enemy_bullet_texture.load_texture(path, w_ren);
}

void Enemies::load_enemies(const char* path, SDL_Renderer* w_ren){

    enemy_texture.load_texture(path, w_ren);
}

void Enemies::load_animations(const char* path, SDL_Renderer* w_ren){

    enemy_explosion_texture.load_texture(path, w_ren);
}

void Enemies::update(SDL_Rect player_pos){

    if(enemies.size() == 0){
        create_new_enemy();
    }

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->update(player_pos);
        if ((*it)->is_alive() == false){
                it = enemies.erase(it);
        }

        else
        {
            ++it;
        }

    }

    counter++;

}

void Enemy::update(SDL_Rect player_pos){

    Entity::update();
    counter++;



    if(player_pos.x > entity_pos.x){
        entity_pos.x += 3;

    }
    if (player_pos.x < entity_pos.x){
        entity_pos.x -= 3;
    }

    if(player_pos.y - entity_pos.y > 100){
        entity_pos.y += 1;

    }
    if (player_pos.y < entity_pos.y){
        entity_pos.y -= 1;
    }

    if(counter == 20){
        e_bullets.shoot(entity_pos);
        counter = 0;
    }

    e_bullets.update();
}


void Enemy::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect) {

    entity_texture.draw(w_ren, d_rect, &entity_pos);
    e_bullets.draw(w_ren, d_rect);

}


