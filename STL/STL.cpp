#include "STL.h"
#include <iostream>



application::application()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << "\n";
		return;
	}
	else
	{
		window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		if (!window)
		{
			std::cout << "Failed to create window\n";
			std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
			return;
		}

		windowSurface = SDL_GetWindowSurface(window);



		if (!windowSurface)
		{
			std::cout << "Failed to get window's surface\n";
			std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
			return;
		}
	}

	

	background = load_bmp("background.bmp");
}

application::~application()
{
	SDL_FreeSurface(windowSurface);
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

		update(1.0 / 60.0);
		draw();
		
	}
}

void application::update(double delta_time)
{
	player.update(delta_time);
	

}

void application::draw()
{
	//SDL_FillRect(windowSurface, nullptr, SDL_MapRGB(windowSurface->format, 0, 0, 0));

	SDL_BlitSurface(background, nullptr, windowSurface, nullptr);

	player.draw(windowSurface);


	SDL_UpdateWindowSurface(window);
}



SDL_Surface *load_bmp(char const *path)
{
	SDL_Surface *optimized_version = nullptr;
	SDL_Surface *image_surface = SDL_LoadBMP(path);

	if (!image_surface)
		return 0;

	optimized_version = SDL_ConvertSurface(image_surface, image_surface->format, 0);

	if (!optimized_version)
	{
		SDL_FreeSurface(image_surface);
		return 0;
	}

	SDL_FreeSurface(image_surface);

	return optimized_version;
}