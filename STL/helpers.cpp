#include "helpers.h"
#include <iostream>

img::img() :
	texture(nullptr) {}

img :: ~img() {
	SDL_DestroyTexture(texture);
}

void img::draw(SDL_Renderer* w_ren, SDL_Rect* s_rect, SDL_Rect* d_rect) {

	SDL_RenderCopy(w_ren, texture, s_rect, d_rect);
}

void img::load_texture(const char* path, SDL_Renderer* w_ren) {

	SDL_Surface* temp = load_bmp(path);
	assert(temp != 0, SDL_GetError());


	texture = SDL_CreateTextureFromSurface(w_ren, temp);
	assert(texture != 0, SDL_GetError());


	SDL_FreeSurface(temp);

}

SDL_Surface *load_bmp(const char* path)
{
	SDL_Surface *image_surface = SDL_LoadBMP(path);
	assert(image_surface != 0, SDL_GetError());
	return image_surface;
}


void assert_func(char const *message, int line, char const *function, char const *file)
{
	std::cout << "ASSERTION FAILED: \n" << "Message: " << message << "\n" << "Line: " << line << "\n" << "Function: " << function << "\n" << "File: " << file << "\n";
	SDL_Quit();
}
