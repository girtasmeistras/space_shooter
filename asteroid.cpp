#include "asteroid.h"
#include "player.h"
#include "stdlib.h"

int Asteroid::count = 0;

Asteroid::Asteroid(): Entity(){

     flag = Flag::HOSTILE;
     health = 50;
     count++;
     entity_pos.x = rand() % (SCREEN_WIDTH - 40);
     entity_pos.y = rand() % (SCREEN_HEIGHT - 40);
}

Asteroid::~Asteroid(){
    count--;
}

void Asteroid::load_powerups(const Image & pwr_txt1, const Image & pwr_txt2){
    powerup_texture1 = pwr_txt1;
    powerup_texture2 = pwr_txt2;
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
    SDL_Rect clip;
    clip.h = EXPLOSION_CLIP;
    clip.w = EXPLOSION_CLIP;
    clip.x = (animation_counter % 4) * EXPLOSION_CLIP;
    clip.y = (animation_counter / 4) * EXPLOSION_CLIP;
    animation_texture.clipped_render(entity_pos.x, entity_pos.y, &clip, w_ren);

    if(animation_counter > 15){
        playing_animation = false;
        int t = rand()%powerups;
        Powerup* to_drop = new Powerup(entity_pos.x, entity_pos.y, t);
        if(t == 0){
            to_drop->load(powerup_texture1);
        }
        if(t == 1){
            to_drop->load(powerup_texture2);
        }
        Entity::entity_list.push_back(to_drop);
    }
}

Powerup::Powerup(int x, int y, int t): Entity(){

    entity_pos.x = x;
    entity_pos.y = y;
    if(t == 0){
        type = PWR_TYPE::HEALTH;
    }
    if(t == 1){
        type = PWR_TYPE::ROCKET;
    }
}

void Powerup::on_collision(Entity* entity){

    Player* player = dynamic_cast<Player*>(entity);
    if(player){
        player->power_up( (int) type);
        alive = false;
    }
}

void Powerup::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

}

