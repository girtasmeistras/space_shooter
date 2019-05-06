#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "entity.h"
#include "asteroid.h"
#include <string>
#include <array>
#include "helpers.h"

/*TODO
    TEXTURES WITH REFERENCE COUNTS FOR AUTOMATIC DELETION - not needed, renderer auto deletes at end
    MORE POWERUPS - done
    LESS BUGS? probably
    MORE SPEED - done
    LESS SPEED - nah
    BOSS - in the future
    valgrind - weird
    */

class Application {

public:
	Application();
	~Application();
    int start();
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
private:
	Player player;
	Enemies enemies;
	SDL_Window*  window;
	SDL_Renderer* window_renderer;
	SDL_Rect window_rect;
	SDL_Event event;
	Image background_texture;
	Image health_texture;
	Image asteroid_texture;
	Image explosion;
	Image rocket_texture;
	SDL_Texture* score_texture;
	SDL_Rect score_rect;
	std::array<SDL_Rect, 4> health_rects;
	const std::string score_str;
    std::string score_count_str;
//UI elements
    int offset;
    int score;
    int new_score;
    int health;
    bool spawned_asteroid;
//const values
    const int SCREEN_FPS = 60;
    const int SCORE_FONT_SIZE = 14;
    const int MESSAGE_FONT_SIZE = 24;
    const int SPAWN_RATE = 3;
};



