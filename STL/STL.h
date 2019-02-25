#pragma once

#include "SDL.h"
#include "gameObject.h"
#include <string>
#include "helpers.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_FPS 60


class application {

public:

	application();
	~application();

	void draw();
	void loop();
	void update();

private:

	gameObject player;

	SDL_Window*  window;
	SDL_Renderer *window_renderer;
	SDL_Texture* background_texture;
	SDL_Rect window_rect;
	SDL_Event   windowEvent;

	


};



