#include "STL.h"
#include <iostream>





application::application()
{
	//Initialize SDL
	assert(SDL_Init(SDL_INIT_VIDEO) > -1, SDL_GetError());
	
	
	window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	assert(window != 0, SDL_GetError());

	window_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(window_renderer != 0, SDL_GetError());
		
	

	background_texture = load_texture("background.bmp", window_renderer);
	window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	player.get_texture(window_renderer);
	player.bullet.get_texture(window_renderer);


	
}

application::~application()
{
	SDL_DestroyTexture(background_texture);
	SDL_DestroyRenderer(window_renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void application::loop()
{

	


	bool keep_window_open = true;
	while (keep_window_open)
	{
		while (SDL_PollEvent(&windowEvent) > 0)
		{
			player.handleEvents(windowEvent);

			switch (windowEvent.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				break;
			}
			
		}

		update();
		draw();
		
	}
}

void application::update()
{
	player.update();
	

}

void application::draw()
{
	SDL_RenderClear(window_renderer);

	SDL_RenderCopy(window_renderer, background_texture, &window_rect, nullptr);

	player.draw(window_renderer, &window_rect);

	SDL_RenderPresent(window_renderer);


	SDL_Delay(1000 / SCREEN_FPS);
}

