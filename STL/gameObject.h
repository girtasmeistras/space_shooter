#pragma once
#include "SDL.h"
#include "helpers.h"
#include "bullet.h"

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


class gameObject {

public:
	gameObject();
	~gameObject();
	void get_texture(SDL_Renderer *window_renderer);
	void update();
	void draw(SDL_Renderer *window_renderer, SDL_Rect* dest_rect);
	void handleEvents(SDL_Event const &event);
	projectile bullet;
	bool bullet_is_present;
	bool space_was_pressed;

private:

	SDL_Texture* p_txt;//player texture
	SDL_Rect     p_pos;//player position
	direction	 pp_dir;//primary player direction
	direction    sp_dir;//secondary player direction
	



};

