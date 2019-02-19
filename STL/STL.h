#pragma once

#include "SDL.h"
#include "gameObject.h"
#include <string>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Surface* load_bmp(char const *path);

class application {

public:

	application();
	~application();

	void draw();
	void loop();
	void update(double delta_time);

private:

	gameObject player;

	SDL_Window*  window;
	SDL_Surface* windowSurface;
	SDL_Surface* background;
	SDL_Event   windowEvent;

	


};



