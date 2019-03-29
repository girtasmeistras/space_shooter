#pragma once
#include "SDL2/SDL.h"
#include <list>
#include <iterator>
#include "helpers.h"
#include "entity.h"

#define BULLET_SPEED 10

class Bullet: public Entity{

public:
	Bullet();
	Bullet(int x, int y, Direction dir);
	~Bullet();
	void update();
private:
    Direction b_direction;
};


class Bullets {
public:
    Bullets() = default;
    ~Bullets() = default;
    void load(const char* path, SDL_Renderer* w_ren);
    void load(Image & texture);
    virtual void draw(SDL_Renderer* w_ren, SDL_Rect* d_rect);
    virtual void update();

protected:
    std::list<Bullet*> bullets;//list of bullets
    Image bullet_texture;
};

class Player_Bullets: public Bullets{

public:
    Player_Bullets() = default;
    ~Player_Bullets() = default;
    void handle_events(const SDL_Event & event, SDL_Rect p_pos);


};

class Enemy_Bullets: public Bullets{

public:
    Enemy_Bullets() = default;
    ~Enemy_Bullets() = default;
    void shoot(SDL_Rect e_pos);

};