#pragma once
#include "SDL2/SDL.h"
#include "helpers.h"
#include "entity.h"
#include "bullet.h"
#include <list>

class Enemy: public Entity{

public:
	Enemy();
	Enemy(Image & enemy_texture, Image & enemy_bullet_texture, Image & enemy_explosion);
	~Enemy();
	void update(SDL_Rect player_pos);
	void load_bullets(Image & bullet_texture);
	void draw(SDL_Renderer* w_ren, SDL_Rect* d_rect);
private:
    Enemy_Bullets e_bullets;
    int counter;
    Image enemy_bullet_texture;
};

class Enemies{
public:
    Enemies(): counter(0){}
    ~Enemies() = default;
    void load_bullets(const char* path, SDL_Renderer* w_ren);
    void load_enemies(const char* path, SDL_Renderer* w_ren);
    void load_animations(const char* path, SDL_Renderer* w_ren);
    void create_new_enemy();
    void update(SDL_Rect player_pos);
private:
    std::list<Enemy*> enemies;
    Image enemy_texture;
    Image enemy_bullet_texture;
    Image enemy_explosion_texture;
    int counter;
};

