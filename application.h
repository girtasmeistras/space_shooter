#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "entity.h"
#include "asteroid.h"
#include <string>
#include <array>
#include <algorithm>
#include "helpers.h"

//valgrind --leak-check=full --show-reachable=yes  --show-possibly-lost=no --error-limit=no --suppressions=/home/deividas/sdl2.supp --log-file=new.out ./main

class Application {

public:
	Application();
	~Application();
    bool start();
	void draw();
	void loop();
	void update();
    void game_over();
    void draw_score();
    void draw_background();
    void draw_entities();
    void draw_health();
    void check_for_deaths();
    void check_for_collisions();
    void spawn_boss();
    void next_level();
    /*TODO
    BOSSES
    LEVELS
    MORE SOUND
    */
    static int level;
private:
	std::shared_ptr<Player> player;
	Enemies enemies;
	deleted_unique_ptr<SDL_Window> window;
	deleted_unique_ptr<SDL_Renderer> window_renderer;
	SDL_Rect window_rect;
	SDL_Event event;
	Image background_texture;
	Image health_texture;
	Image asteroid_texture;
	Image explosion;
	Image rocket_texture;
	deleted_unique_ptr<SDL_Texture> score_texture;
	SDL_Rect score_rect;
	std::array<SDL_Rect, 4> health_rects;
	const std::string score_str;
    deleted_unique_ptr<Mix_Chunk> ok;
    deleted_unique_ptr<Mix_Music> music;
    std::string score_count_str;
    Texture_Manager texture_man;
//UI elements
    int offset;
    int score;
    int new_score;
    int health;
    bool spawned_asteroid;
    bool changed_level;
//const values
    const int SCREEN_FPS = 60;
    const int SCORE_FONT_SIZE = 14;
    const int MESSAGE_FONT_SIZE = 24;
    const int SPAWN_RATE = 2;
};


struct completely_dead{
    bool operator()(std::shared_ptr<Entity> & e){
        if(e->is_alive())
            return false;
        if(e->is_playing_animation())
            return false;

        return true;
    }
};

struct can_get_score{
    bool operator()(std::shared_ptr<Entity> & e){
        if(e->is_alive() || !e->is_enemy()){
            return false;
        }
        if(!e->already_got_score())
            return true;

        return false;
    }
};
