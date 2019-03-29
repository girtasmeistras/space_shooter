#pragma once

#define assert(expression, message) if(expression == false) assert_func(message, __LINE__, __func__, __FILE__)

#include "SDL.h"


class img {

public:
	img();
	~img();
	void draw(SDL_Renderer* w_ren, SDL_Rect* s_rect, SDL_Rect* d_rect);
	void load_texture(const char* path, SDL_Renderer* w_ren);

private:
	SDL_Texture* texture;

};


SDL_Surface* load_bmp(const char* path);
void assert_func(char const *message, int line, char const *function, char const *file);