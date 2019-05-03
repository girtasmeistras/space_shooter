#pragma once

#define assert(expression, message) if(expression == false) assert_func(message, __LINE__, __func__, __FILE__)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include "SDL2/SDL.h"
#include<SDL2/SDL_image.h>
#include "SDL2/SDL_ttf.h"
#include<map>

class Image {

public:
	Image();
	~Image();
	void destroy();
	void draw(SDL_Renderer* w_ren, SDL_Rect* s_rect, SDL_Rect* d_rect) const;
	void clipped_render( int x, int y, SDL_Rect* clip, SDL_Renderer* w_ren) const;
	void load_texture(const char* path, SDL_Renderer* w_ren);
    SDL_Texture* get_texture() const;
private:
	SDL_Texture* texture;
};

SDL_Surface* load_bmp(const char* path);
SDL_Surface* load_png(const char* path);
void assert_func(char const *message, int line, char const *function, char const *file);
SDL_Texture* get_font_texture(const char* message, int font_size, SDL_Rect* message_rect, SDL_Renderer* window_renderer);

