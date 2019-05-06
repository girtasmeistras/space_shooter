#pragma once
#include "SDL2/SDL.h"
#include <list>
#include <iterator>
#include "helpers.h"
#include "entity.h"

class Bullet: public Entity{

public:
	Bullet();
	Bullet(int x, int y, Direction dir, const Image & texture);
	~Bullet() = default;
	void update();
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect) { }
	void on_collision(Entity* entity){alive = false;}
private:
    Direction b_direction;
    const int BULLET_SPEED = 10;
    const int ENEMY_BULLET_SPEED = 5;
};

class Rocket: public Bullet{

public:
	Rocket(): Bullet() {}
	Rocket(int x, int y, Direction dir, const Image & texture): Bullet(x,y,dir,texture){ }
	~Rocket()=default;
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect) { }
	void on_collision(Entity* entity){ /*go straight through enemies */ }
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
    std::list<Bullet*> bullets;//list of bullets
    Image bullet_texture;
};

class Player_Bullets: public Bullets{

public:
    Player_Bullets() = default;
    ~Player_Bullets() = default;
    void handle_events(const SDL_Event & event, const SDL_Rect & p_pos, bool & has_rocket);
    void load_nuke(const char* path, SDL_Renderer* w_ren);
    Image nuke;
};

class Enemy_Bullets: public Bullets{

public:
    Enemy_Bullets() = default;
    ~Enemy_Bullets() = default;
    void shoot(const SDL_Rect & e_pos);

};
