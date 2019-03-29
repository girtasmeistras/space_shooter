#pragma once
#include "SDL.h"
#include "helpers.h"
#include "bullet.h"
#include <list>
#include <iterator>
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

enum class direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
	
};


class Player {

public:
	Player();
	~Player();
	void update();
	void draw(SDL_Renderer* w_ren, SDL_Rect* d_rect, img* b_txt);
	void handleEvents(SDL_Event const &event);
	void get_texture(SDL_Renderer* w_ren);

private:
	img p_txt;//Player texture
	SDL_Rect p_pos;//Player position
	direction pp_dir;//primary Player direction
	direction sp_dir;//secondary Player direction
	std::list<Bullet*> bullets;//list of bullets



};

