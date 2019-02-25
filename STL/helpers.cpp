#include "helpers.h"
#include <iostream>

SDL_Surface *load_bmp(const char* path)
{
	SDL_Surface *optimized_version = nullptr;
	SDL_Surface *image_surface = SDL_LoadBMP(path);

	assert(image_surface != 0, SDL_GetError());

	optimized_version = SDL_ConvertSurface(image_surface, image_surface->format, 0);

	assert(optimized_version != 0, SDL_GetError());

	SDL_FreeSurface(image_surface);

	return optimized_version;
}


SDL_Texture* load_texture(const char* path, SDL_Renderer* window_renderer) {

	SDL_Surface* temp = load_bmp(path);
	assert(temp != 0, SDL_GetError());

	SDL_Texture* new_texture = SDL_CreateTextureFromSurface(window_renderer, temp);
	assert(new_texture != 0, SDL_GetError());

	SDL_FreeSurface(temp);
	return new_texture;

}

void assert_func(char const *message, int line, char const *function, char const *file)
{
	std::cout << "ASSERTION FAILED: \n" << "Message: " << message << "\n" << "Line: " << line << "\n" << "Function: " << function << "\n" << "File: " << file << "\n";
	SDL_Quit();
}