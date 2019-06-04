#pragma once
#include "SDL2/SDL.h"
#include "helpers.h"
#include "entity.h"
#include "bullet.h"
#include <list>

const int BASIC_ENEMY_SPEED = 2;
const int BOSS_ENEMY_SPEED = 1;


class Enemy: public Entity{

public:
	Enemy();
	Enemy(const SDL_Rect & player_pos, const Image & enemy_texture, const Image & enemy_bullet_texture, const Image & enemy_explosion);
	~Enemy();
	virtual void update(const SDL_Rect & player_pos, bool changed_pos);
	void load_bullets(const Image & bullet_texture);
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity);
	void kill_bullets();
	bool is_enemy() const {return true;}
protected:
    Enemy_Bullets e_bullets;
    int bullet_delay;
    Image enemy_bullet_texture;
};

class Weird_Enemy: public Enemy{
public:
	Weird_Enemy(): Enemy() {}
	Weird_Enemy(const SDL_Rect & player_pos, const Image & enemy_texture, const Image & enemy_bullet_texture, const Image & enemy_explosion):
	    Enemy(player_pos, enemy_texture, enemy_bullet_texture, enemy_explosion) {}
	void update(const SDL_Rect & player_pos, bool changed_pos);
};

class Boss: public Enemy{

public:
	Boss(): Enemy() {}
	Boss(const SDL_Rect & player_pos, const Image & enemy_texture, const Image & enemy_bullet_texture, const Image & enemy_explosion):
	    Enemy(player_pos, enemy_texture, enemy_bullet_texture, enemy_explosion) {health = 150; entity_pos.x = SCREEN_WIDTH/2; entity_pos.y = 20;}
	void update(const SDL_Rect & player_pos, bool changed_pos);
};

class Enemies{
public:
    Enemies(): player_pos{0,0,0,0} { }
    ~Enemies() = default;
    void load_bullets(const Image & texture);
    void load_enemies(const Image & texture);
    void load_boss(const Image & texture);
    void load_animations(const Image & texture);
    void create_new_enemy(const SDL_Rect & player_pos);
    void create_weird_enemy(const SDL_Rect & player_pos);
    void spawn_boss(const SDL_Rect & p_pos);
    void update(const SDL_Rect & p_pos);
    void restart();
    static int wave;
private:
    std::list<std::shared_ptr<Enemy>> enemies;
    Image enemy_texture;
    Image boss_texture;
    Image enemy_bullet_texture;
    Image enemy_explosion_texture;
    SDL_Rect player_pos;
};

