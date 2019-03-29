#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "entity.h"
#include <string>
#include "helpers.h"

#define SCREEN_FPS 60
#define SCORE_FONT_SIZE 14

class Application {

public:

	Application();
	~Application();
    int start();
	void draw();
	void loop();
	void update();


private:

	Player player;
	Enemies enemies;
	SDL_Window*  window;
	SDL_Renderer* window_renderer;
	SDL_Rect window_rect;
	SDL_Event event;
	Image background_texture;
	Image health_texture;
	SDL_Texture* score_texture;
	SDL_Rect score_rect;
	SDL_Rect health_rect;
	std::string score_str;
    std::string score_count_str;
    int offset;
    int score;
    int new_score;
};



