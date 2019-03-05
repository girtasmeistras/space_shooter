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


class player {

public:
	player();
	~player();
	void update();
	void draw(SDL_Renderer* window_renderer, SDL_Rect* d_rect, img* bullet_texture);
	void handleEvents(SDL_Event const &event);
	void get_texture(SDL_Renderer* window_renderer);

private:
	img p_txt;//player texture
	SDL_Rect p_pos;//player position
	direction pp_dir;//primary player direction
	direction sp_dir;//secondary player direction
	std::list<Bullet*> bullets;//list of bullets



};

