#include "enemy.h"
#include "application.h"
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

Enemy::~Enemy() {
    kill_bullets();
}

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
            e_bullets.shoot(entity_pos, false);
            bullet_delay = 0;
        }
    }
    e_bullets.update();
}

void Weird_Enemy::update(const SDL_Rect & player_pos, bool changed_pos){

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
            e_bullets.shoot(entity_pos, true);
            bullet_delay = 0;
        }
    }
    e_bullets.update();
}

void Boss::update(const SDL_Rect & player_pos, bool changed_pos){
    Entity::update();
    bullet_delay++;
    if(player_pos.x > entity_pos.x){
        entity_pos.x += BOSS_ENEMY_SPEED;
    }
    if(player_pos.x < entity_pos.x){
        entity_pos.x -= BOSS_ENEMY_SPEED;
    }
    if(player_pos.y > entity_pos.y){
        entity_pos.y += BOSS_ENEMY_SPEED;
    }
    if(player_pos.y < entity_pos.y){
        entity_pos.y -= BOSS_ENEMY_SPEED;
    }
    if(bullet_delay >= 20){
        SDL_Rect sec_pos1 = entity_pos;
        SDL_Rect sec_pos2 = entity_pos;
        sec_pos1.x -= entity_pos.w/3;
        sec_pos2.x += entity_pos.w/3;
        e_bullets.shoot(entity_pos, false);
        e_bullets.shoot(sec_pos1, false);
        e_bullets.shoot(sec_pos2, false);
        bullet_delay = 0;
    }
    e_bullets.update();
}

void Enemy::kill_bullets() {

    e_bullets.kill();
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

void Enemies::restart(){
    wave = 1;
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->kill();
        (*it)->kill_bullets();
        it = enemies.erase(it);
        //++it;
    }
}

void Enemies::create_new_enemy(const SDL_Rect & player_pos){
    auto new_enemy = std::make_shared<Enemy>(player_pos, enemy_texture, enemy_bullet_texture, enemy_explosion_texture);
    enemies.push_back(new_enemy);
    Entity::entity_list.push_back(std::move(new_enemy));
}

void Enemies::create_weird_enemy(const SDL_Rect & player_pos){
    auto new_weird_enemy = std::make_shared<Weird_Enemy>(player_pos, enemy_texture, enemy_bullet_texture, enemy_explosion_texture);
    enemies.push_back(new_weird_enemy);
    Entity::entity_list.push_back(std::move(new_weird_enemy));
}

void Enemies::spawn_boss(const SDL_Rect & player_pos){
    auto new_boss = std::make_shared<Boss>(player_pos, boss_texture, enemy_bullet_texture, enemy_explosion_texture);
    enemies.push_back(new_boss);
    Entity::entity_list.push_back(std::move(new_boss));
}

void Enemies::load_bullets(const Image & texture){

    enemy_bullet_texture = texture;
}

void Enemies::load_enemies(const Image & texture){

    enemy_texture = texture;
}

void Enemies::load_boss(const Image & texture){

    boss_texture = texture;
}

void Enemies::load_animations(const Image & texture){

    enemy_explosion_texture = texture;
}

void Enemies::update(const SDL_Rect & p_pos)
{
    bool pos_changed = false;
    if(p_pos.x != player_pos.x || p_pos.y != player_pos.y){
        pos_changed = true;
        player_pos = p_pos;
    }
    if(wave % 4 == 0 && enemies.size() == 0){
        spawn_boss(player_pos);
        ++wave;
    }
    if(enemies.size() == 0){
        create_weird_enemy(player_pos);
        for(int i = 1; i < wave; i++){
            create_new_enemy(player_pos);
        }
        ++wave;
    }
    if(wave%3 == 0){
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
