#pragma once
#include "SDL2/SDL.h"
#include <list>
#include <iterator>
#include "helpers.h"
#include "entity.h"

class Powerup: public Entity{

public:
    Powerup(): Entity() {}
	Powerup(int x, int y);
	~Powerup() = default;
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity);

};

class Asteroid: public Entity{

public:
	Asteroid();
	~Asteroid();
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity);
	void load_powerup(const Image & pwr_txt);
    static int count;
private:
    Image powerup_texture;
};


