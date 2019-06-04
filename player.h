#pragma once
#include "SDL2/SDL.h"
#include "helpers.h"
#include "entity.h"
#include "bullet.h"
const float SPEED = 450;

class Player: public Entity{

public:
	Player();
	~Player();
	void update();
	void handle_events(const SDL_Event & event);
	void on_collision(Entity* entity);
    void draw_animation(SDL_Renderer* w_ren, SDL_Rect* d_rect);
    void load_bullets(const Image & texture);
    void load_nuke(const Image & texture);
    int get_health() const;
    void restart();
    void power_up(int type);
    bool is_player() const {return true;}
private:
	Direction pp_dir;//primary Player direction
	Direction sp_dir;//secondary Player direction
    Player_Bullets p_bullets;
    bool has_rocket;
    deleted_unique_ptr<Mix_Chunk> health_sound;
    vec2d<float> movement_pos;
};

