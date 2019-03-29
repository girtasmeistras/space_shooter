#pragma once
#include "SDL2/SDL.h"
#include "helpers.h"
#include "entity.h"
#include "bullet.h"
#define SPEED 6



class Player: public Entity{

public:
	Player();
	~Player();
	void update();
	void handle_events(const SDL_Event & event);
    void draw(SDL_Renderer* w_ren, SDL_Rect* d_rect);
    void load_bullets(const char* path, SDL_Renderer* w_ren);
    int get_health();

private:
	Direction pp_dir;//primary Player direction
	Direction sp_dir;//secondary Player direction
    Player_Bullets p_bullets;


};

