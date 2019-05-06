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
	void on_collision(Entity* entity);
    void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
    void load_bullets(const char* path, SDL_Renderer* w_ren);
    void load_nuke(const char* path, SDL_Renderer* w_ren);
    int get_health() const;
    void restart();
    void power_up(int type);
private:
	Direction pp_dir;//primary Player direction
	Direction sp_dir;//secondary Player direction
    Player_Bullets p_bullets;
    bool has_rocket;
};

