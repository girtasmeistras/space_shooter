#pragma once
#include "helpers.h"
#include "SDL2/SDL.h"
#include <vector>

const int BULLET_DAMAGE = 25;
const int EXPLOSION_CLIP = 64;

enum class Flag{

    FRIENDLY,
    HOSTILE
};

enum class Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT

};

class Entity {
public:
    static std::vector<Entity*> entity_list;
    Entity();
    virtual ~Entity();
    void load(const char* path, SDL_Renderer* w_ren);
    void load(const Image & texture);
    void load_animation(const Image & sprite_texture);
    void load_animation(const char* path, SDL_Renderer* w_ren);
    virtual void update();
    virtual void draw(SDL_Renderer* w_ren, SDL_Rect* d_rect);
    virtual void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect) = 0;
    virtual void on_collision(Entity* entity);
    bool collides(const SDL_Rect & sec_entity_pos) const;
    void check_for_collisions();
    void clean_up();
    void kill();
    const SDL_Rect & get_pos() const;
    bool is_alive() const;
    int get_flag() const;
    bool is_playing_animation() const { return playing_animation;}
    bool already_got_score();
protected:
    Image entity_texture;
    Image animation_texture;
    SDL_Rect entity_pos;
    Flag flag;
    bool alive;
    bool playing_animation;
    bool got_score;
    int health;
    int animation_counter;
};

class Entity_Collision {
public:
    static std::vector<Entity_Collision> entity_collision_list;
    Entity* first_entity;
    Entity* second_entity;
    Entity_Collision();
};
