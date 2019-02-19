#pragma once
#include "SDL.h"

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


class projectile {

public:
	projectile();
	projectile(double x, double y);
	~projectile() = default;

	void update(double delta_time);
	void draw(SDL_Renderer window_renderer);

private:
	SDL_Surface *m_image;
	SDL_Rect     m_position;
	double       m_x;
	double       m_y;
	

};


class gameObject {

public:
	gameObject();
	~gameObject() = default;

	void update(double delta_time);
	void draw(SDL_Surface *window_surface);
	void handleEvents(SDL_Event const &event);

private:

	SDL_Surface* player_surface;
	SDL_Rect     m_position;
	double       m_x;
	double       m_y;
	direction	 m_direction;
	direction    s_direction;
	



};

