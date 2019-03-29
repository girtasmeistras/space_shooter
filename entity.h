#pragma once
#include "helpers.h"
#include "SDL2/SDL.h"
#include <vector>

#define BULLET_DAMAGE 25
#define ENEMY_CLIP 64

enum class Flag{

    DEFAULT,
    BULLET,
    PLAYER,
    ENEMY

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

    protected:
        Image entity_texture;
        Image animation_texture;
        SDL_Rect entity_pos;
        Flag flag;
        bool alive;
        bool hurts_player;
        bool playing_animation;
        int probe_times;
        int health;
        int animation_counter;
    public:
        Entity();

        virtual ~Entity();

    public:
        virtual void load(const char* path, SDL_Renderer* w_ren);
        virtual void load(Image & texture);
        void load_animation(Image & sprite_texture);
        virtual void update();

        virtual void draw(SDL_Renderer* w_ren, SDL_Rect* d_rect);
        void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
        virtual void on_collision(Entity* entity);

        virtual bool collides(SDL_Rect sec_entity_pos);

        virtual bool is_playing_animation(){ return playing_animation;}
        int was_already_probed(){
            probe_times++;
            return probe_times;
            }
        virtual void check_for_collisions();

        virtual void clean_up();

        SDL_Rect get_pos();
        bool is_alive();
        int get_flag();
};

class Entity_Collision {
    public:
        static std::vector<Entity_Collision>    entity_collision_list;

    public:
        Entity* first_entity;
        Entity* second_entity;

    public:
        Entity_Collision();
};
