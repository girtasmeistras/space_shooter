#pragma once
#include "SDL2/SDL.h"
#include <list>
#include <iterator>
#include "helpers.h"
#include "entity.h"

const int powerups = 2;

enum class PWR_TYPE{

    HEALTH,
    ROCKET
};

class Powerup: public Entity{

public:
    Powerup(): Entity() {}
	Powerup(int x, int y, int t);
	~Powerup() = default;
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity);
	bool is_powerup() const {return true;}
private:
    PWR_TYPE type;
};

class Asteroid: public Entity{

public:
	Asteroid();
	~Asteroid();
	void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
	void on_collision(Entity* entity);
	void load_powerups(const Image & pwr_txt1, const Image & pwr_txt2);
    static int count;
private:
    Image powerup_texture1;
    Image powerup_texture2;
};


