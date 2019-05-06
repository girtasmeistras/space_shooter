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

    flag = Flag::FRIENDLY;
    alive = true;
    health = 100;
    animation_counter = -1;
    playing_animation = false;
    got_score = false;
}

Entity::~Entity() {

}

void Entity::load(const char* path, SDL_Renderer* w_ren){

    entity_texture.load_texture(path, w_ren);
    SDL_Texture* temp = entity_texture.get_texture();
    SDL_QueryTexture(temp, NULL, NULL, &entity_pos.w, &entity_pos.h);
}

void Entity::load( const Image & texture){

    entity_texture = texture;
    SDL_Texture* temp = entity_texture.get_texture();
    SDL_QueryTexture(temp, NULL, NULL, &entity_pos.w, &entity_pos.h);
}

void Entity::load_animation(const Image & sprite_texture){

    animation_texture = sprite_texture;

}

void Entity::load_animation(const char* path, SDL_Renderer* w_ren){

    animation_texture.load_texture(path, w_ren);
}

void Entity::update() {

    check_for_collisions();
}

void Entity::draw(SDL_Renderer* w_ren, SDL_Rect* d_rect){
    if(alive){
        entity_texture.draw(w_ren, d_rect, &entity_pos);
    }
}


const SDL_Rect & Entity::get_pos() const{

    return entity_pos;
}

bool Entity::is_alive() const{
    return alive;
}

bool Entity::already_got_score(){
    bool to_return = got_score;
    got_score = true;
    return to_return;
}

int Entity::get_flag() const{

    return (int) flag;
}

void Entity::clean_up() {

    entity_texture.destroy();
    animation_texture.destroy();
}

void Entity::kill() {
    got_score = true;
    alive = false;
}


void Entity::on_collision(Entity* entity){

    health -= BULLET_DAMAGE;

 }


bool Entity::collides(const SDL_Rect & sec_entity_pos) const{

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

    for(unsigned int i = 0;i < entity_list.size();i++) {
        if(this != entity_list[i] && entity_list[i] != nullptr && alive && entity_list[i]->is_alive() == true &&collides(entity_list[i]->entity_pos) == true) {

            Entity_Collision collision;
            collision.first_entity = this;
            collision.second_entity = entity_list[i];
            Entity_Collision::entity_collision_list.push_back(collision);
        }
    }
}
