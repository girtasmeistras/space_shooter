#pragma once

#define assert(expression, message) if(expression == false) assert_func(message, __LINE__, __func__, __FILE__)

#include "SDL.h"

SDL_Surface* load_bmp(const char* path);
SDL_Texture* load_texture(const char* path, SDL_Renderer* window_renderer);
void assert_func(char const *message, int line, char const *function, char const *file);