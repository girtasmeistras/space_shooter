#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <list>
#include <iterator>
#include <algorithm>
#include "helpers.h"
#include "entity.h"

class Bullet: public Entity{

public:
	Bullet();
	Bullet(const SDL_Rect & start_pos, Direction p_dir, Direction s_dir, bool friendly, const Image & texture);
	~Bullet() = default;
	virtual void update();
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect) { }
	void on_collision(Entity* entity){alive = false;}
protected:
    Direction p_b_direction;//primary direction
    Direction s_b_direction;//secondary direction
    const int BULLET_SPEED = 10;
    const int ENEMY_BULLET_SPEED = 5;
};

class Acc_Bullet: public Bullet{
public:
    Acc_Bullet(): Bullet(), speed(1), counter(0) {}
    Acc_Bullet(const SDL_Rect & start_pos, Direction p_dir, Direction s_dir, bool friendly, const Image & texture):
        Bullet(start_pos, p_dir, s_dir, friendly, texture), speed(0), counter(0) { entity_pos.y += BULLET_SPEED;}
    void update();
private:
    int speed;
    int counter;
    const int ACCELERATION = 1;

};

class Rocket: public Bullet{

public:
	Rocket(): Bullet(), counter(0) {}
	Rocket(const SDL_Rect & start_pos, Direction p_dir, Direction s_dir, bool friendly, const Image & texture);
	~Rocket() = default;
	void update();
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity){ }
private:
    int counter;
    deleted_unique_ptr<Mix_Chunk> explosion_sound;
};


class Bullets {
public:
    Bullets() = default;
    ~Bullets() = default;
    void load(const char* path, SDL_Renderer* w_ren);
    void load(const Image & texture);
    void update();
    void kill();
protected:
    std::list<std::shared_ptr<Bullet>> bullets;//list of bullets
    Image bullet_texture;
};

class Player_Bullets: public Bullets{

public:
    Player_Bullets() = default;
    ~Player_Bullets() = default;
    void handle_events(const SDL_Event & event, const SDL_Rect & p_pos, bool & has_rocket);
    void load_nuke(const Image & texture);
    void load_explosion(const Image & texture);
private:
    Image nuke;
    Image explosion;
    deleted_unique_ptr<Mix_Chunk> nuke_sound;
    deleted_unique_ptr<Mix_Chunk> bullet_sound;
};

class Enemy_Bullets: public Bullets{

public:
    Enemy_Bullets() = default;
    ~Enemy_Bullets() = default;
    void shoot(const SDL_Rect & e_pos, bool accelerate);

};
