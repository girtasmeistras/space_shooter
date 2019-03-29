#include <iostream>
#include "entity.h"
std::vector<Entity*> Entity::entity_list;
std::vector<Entity_Collision> Entity_Collision::entity_collision_list;

Entity_Collision::Entity_Collision() {

    first_entity = nullptr;
    second_entity = nullptr;
}


Entity::Entity() {

    entity_pos.x = 0;
    entity_pos.y = 0;
    entity_pos.w = 0;
    entity_pos.h = 0;

    flag = Flag::DEFAULT;
    alive = true;
    hurts_player = true;
    health = 100;
    animation_counter = -1;
    playing_animation = false;
    probe_times = 0;
}

Entity::~Entity() {

}

void Entity::load(const char* path, SDL_Renderer* w_ren){

    entity_texture.load_texture(path, w_ren);
    SDL_Texture* temp = entity_texture.get_texture();

    SDL_QueryTexture(temp, NULL, NULL, &entity_pos.w, &entity_pos.h);

}

void Entity::load(Image & texture){

    entity_texture = texture;
    SDL_Texture* temp = entity_texture.get_texture();

    SDL_QueryTexture(temp, NULL, NULL, &entity_pos.w, &entity_pos.h);

}

void Entity::load_animation(Image & sprite_texture){

    animation_texture = sprite_texture;

}

void Entity::update() {

    check_for_collisions();

}

void Entity::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect) {

        entity_texture.draw(w_ren, d_rect, &entity_pos);
}

void Entity::draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect){

    if(flag == Flag::ENEMY){

        ++animation_counter;
        int x = animation_counter % 4;
        int y = animation_counter / 4;
        SDL_Rect clip;
        clip.h = ENEMY_CLIP;
        clip.w = ENEMY_CLIP;
        clip.x = (animation_counter % 4) * ENEMY_CLIP;
        clip.y = (animation_counter / 4) * ENEMY_CLIP;
        animation_texture.clipped_render(entity_pos.x, entity_pos.y, &clip, w_ren);

        if(animation_counter > 15){
            playing_animation = false;
        }

    }
}

SDL_Rect Entity::get_pos(){

    return entity_pos;
}

bool Entity::is_alive(){
    return alive;
}

int Entity::get_flag() {

    return (int) flag;
}

void Entity::clean_up() {

    entity_texture.destroy();
    animation_texture.destroy();
}

void Entity::on_collision(Entity* entity){

    if( flag == Flag::BULLET){
        alive = false;
    }

    health -= BULLET_DAMAGE;

    if( flag == Flag::ENEMY){
        if(health <= 0){
            alive = false;
            playing_animation = true;
        }
    }
 }

bool Entity::collides(SDL_Rect sec_entity_pos) {

    int x_offset1 = entity_pos.w / 10;
    int x_offset2 = sec_entity_pos.w / 10;
    int y_offset1 = entity_pos.h / 10;
    int y_offset2 = sec_entity_pos.h / 10;

    int left1 = entity_pos.x + x_offset1;
    int left2 = sec_entity_pos.x + x_offset2;

    int right1 = entity_pos.x + entity_pos.w - x_offset1;
    int right2 = sec_entity_pos.x + sec_entity_pos.w - x_offset2;

    int top1 = entity_pos.y + y_offset1;
    int top2 = sec_entity_pos.y + y_offset2;

    int bottom1 = entity_pos.y + entity_pos.h - y_offset1;
    int bottom2 = sec_entity_pos.y  + sec_entity_pos.h - y_offset2;


    if (bottom1 < top2) return false;
    if (top1 > bottom2) return false;

    if (right1 < left2) return false;
    if (left1 > right2) return false;

    return true;
}

void Entity::check_for_collisions() {

    for(int i = 0;i < entity_list.size();i++) {

        if(this != entity_list[i] && entity_list[i] != nullptr && this->is_alive() == true && entity_list[i]->is_alive() == true &&collides(entity_list[i]->entity_pos) == true) {


            Entity_Collision collision;

            collision.first_entity = this;
            collision.second_entity = entity_list[i];

            Entity_Collision::entity_collision_list.push_back(collision);


        }
    }

}
