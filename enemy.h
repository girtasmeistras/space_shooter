#pragma once
#include "SDL2/SDL.h"
#include "helpers.h"
#include "entity.h"
#include "bullet.h"
#include <list>

class Enemy: public Entity{

public:
	Enemy();
	Enemy(const SDL_Rect & player_pos, const Image & enemy_texture, const Image & enemy_bullet_texture, const Image & enemy_explosion);
	~Enemy();
	void update(const SDL_Rect & player_pos, bool changed_pos);
	void load_bullets(const Image & bullet_texture);
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity);
	void kill_bullets();
private:
    Enemy_Bullets e_bullets;
    int bullet_delay;
    Image enemy_bullet_texture;
};

class Enemies{
public:
    Enemies() = default;
    ~Enemies() = default;
    void load_bullets(const char* path, SDL_Renderer* w_ren);
    void load_enemies(const char* path, SDL_Renderer* w_ren);
    void load_animations(const Image & texture);
    void create_new_enemy(const SDL_Rect & player_pos);
    void update(const SDL_Rect & p_pos);
    void restart();
    static int wave;
private:
    std::list<Enemy*> enemies;
    Image enemy_texture;
    Image enemy_bullet_texture;
    Image enemy_explosion_texture;
    SDL_Rect player_pos;
};

