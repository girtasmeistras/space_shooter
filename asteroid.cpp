#include "asteroid.h"
#include "player.h"
#include "stdlib.h"

int Asteroid::count = 0;

Asteroid::Asteroid(): Entity(){
     flag = Flag::HOSTILE;
     health = 50;
     count++;
     entity_pos.x = rand() % (SCREEN_WIDTH - entity_pos.w);
     entity_pos.y = rand() % (SCREEN_HEIGHT - entity_pos.h);
}

Asteroid::~Asteroid(){
    count--;
    Powerup* to_drop = new Powerup(entity_pos.x, entity_pos.y);
    to_drop->load(powerup_texture);
    Entity::entity_list.push_back(to_drop);
}

void Asteroid::on_collision(Entity* entity){
    Entity::on_collision(entity);
    if(health <= 0){
        playing_animation = true;
        alive = false;
    }
}

void Asteroid::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

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

Powerup::Powerup(int x, int y): Entity(){

    entity_pos.x = x;
    entity_pos.y = y;
}

void Powerup::on_collision(Entity* entity){

    Player* player = dynamic_cast<Player*>(entity);
    if(player){
        player->power_up();
        alive = false;
    }
}

void Powerup::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){


}

void Asteroid::load_powerup(const Image & pwr_txt){
    powerup_texture = pwr_txt;
}
