#include "enemy.h"
#include <iostream>
#include "stdlib.h"

int Enemies::wave = 1;

Enemy::Enemy(): Entity(), bullet_delay(0) {
    flag = Flag::HOSTILE;

}

Enemy::Enemy(const SDL_Rect & player_pos, const Image & enemy_texture, const Image & enemy_bullet_texture, const Image & enemy_explosion): Entity(), bullet_delay(0){

    flag = Flag::HOSTILE;
    load(enemy_texture);
    load_animation(enemy_explosion);
    load_bullets(enemy_bullet_texture);
    health = 25;

    entity_pos.x = rand()%SCREEN_WIDTH;
    entity_pos.y = rand()%SCREEN_HEIGHT-100;
    if(std::abs(player_pos.x - entity_pos.x) < 50){
        entity_pos.x = rand()%SCREEN_WIDTH;
    }
    if(std::abs(player_pos.y - entity_pos.y) < 50){
        entity_pos.y = rand()%SCREEN_HEIGHT-100;
    }
}

Enemy::~Enemy() {}

void Enemy::load_bullets(const Image & bullet_texture){

    e_bullets.load(bullet_texture);
}

void Enemy::update(const SDL_Rect & player_pos, bool changed_pos){

    Entity::update();
    bullet_delay++;
    if(changed_pos){
        if(player_pos.x > entity_pos.x){
            entity_pos.x += 2;
        }
        if (player_pos.x < entity_pos.x){
            entity_pos.x -= 2;
        }
        if(player_pos.y - entity_pos.y > 50){
            entity_pos.y += 2;
        }
        else{
            entity_pos.y -= 2;
        }
        if (player_pos.y - entity_pos.y < -50){
            entity_pos.y -= 2;
        }
        else{
            entity_pos.y += 2;
        }
        if(bullet_delay >= 50){
            e_bullets.shoot(entity_pos);
            bullet_delay = 0;
        }
    }
    e_bullets.update();
}

void Enemy::kill_bullets() {

    e_bullets.kill();
}

void Enemy::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect) {

    entity_texture.draw(w_ren, d_rect, &entity_pos);
    e_bullets.draw(w_ren, d_rect);
}

void Enemy::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

    ++animation_counter;
    SDL_Rect clip;
    clip.h = EXPLOSION_CLIP;
    clip.w = EXPLOSION_CLIP;
    clip.x = (animation_counter % 4) * EXPLOSION_CLIP;
    clip.y = (animation_counter / 4) * EXPLOSION_CLIP;
    animation_texture.clipped_render(entity_pos.x, entity_pos.y, &clip, w_ren);

    if(animation_counter > 15){
        playing_animation = false;
    }
}

void Enemy::on_collision(Entity* entity){
    Entity::on_collision(entity);
     if(health <= 0){
        alive = false;
        playing_animation = true;
    }
}

Enemies::~Enemies(){
    enemy_texture.destroy();
    enemy_bullet_texture.destroy();
    enemy_explosion_texture.destroy();
}

void Enemies::restart(){//TODO
    wave = 1;
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->kill_bullets();
        (*it)->kill();
        ++it;
    }
}

void Enemies::create_new_enemy(const SDL_Rect & player_pos){

    Enemy* new_enemy = new Enemy(player_pos, enemy_texture, enemy_bullet_texture, enemy_explosion_texture);
    enemies.push_back(new_enemy);
    Entity::entity_list.push_back(new_enemy);
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

void Enemies::update(const SDL_Rect & p_pos)
{
    bool pos_changed = false;
    if(p_pos.x != player_pos.x || p_pos.y != player_pos.y){
        pos_changed = true;
        player_pos = p_pos;
    }
    if(enemies.size() == 0){
        for(int i = 0; i < wave; i++){
            create_new_enemy(player_pos);
        }
        ++wave;
    }
    if((wave-1)%3 == 0){
        pos_changed = true;
    }
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it)->is_alive() == false){
                (*it)->kill_bullets();
                it = enemies.erase(it);
        }
        else{
            (*it)->update(player_pos, pos_changed);
            ++it;
        }
    }
}
