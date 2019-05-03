#include "helpers.h"
#include <iostream>
#include <string>

Image::Image() :
	texture(nullptr) {}

Image :: ~Image() {

}

void Image::destroy(){
    if(texture != nullptr){
        SDL_DestroyTexture(texture);
    }
}


void Image::draw(SDL_Renderer* w_ren, SDL_Rect* s_rect, SDL_Rect* d_rect) const{

	SDL_RenderCopy(w_ren, texture, s_rect, d_rect);
}


void Image::clipped_render( int x, int y, SDL_Rect* clip, SDL_Renderer* w_ren) const
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT };

    if( clip != nullptr )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    /*
	//Set clip rendering dimensions
    renderQuad.w = SCREEN_WIDTH;
    renderQuad.h = SCREEN_HEIGHT; */
    SDL_RendererFlip flip = SDL_FLIP_NONE;

	//Render to screen
	SDL_RenderCopyEx( w_ren, texture, clip, &renderQuad, 0, 0, flip);
}

SDL_Texture* Image::get_texture() const{

    return texture;
}

void Image::load_texture(const char* path, SDL_Renderer* w_ren) {

	SDL_Surface* temp = load_png(path);
	assert(temp != 0, SDL_GetError());


	texture = SDL_CreateTextureFromSurface(w_ren, temp);
	assert(texture != 0, SDL_GetError());


	SDL_FreeSurface(temp);

}

SDL_Surface* load_bmp(const char* path)
{
	SDL_Surface *image_surface = SDL_LoadBMP(path);
	assert(image_surface != 0, SDL_GetError());
	return image_surface;
}


void assert_func(char const *message, int line, char const *function, char const *file)
{
	std::cout << "ASSERTION FAILED: \n" << "Message: " << message << "\n" << "Line: " << line << "\n" << "Function: " << function << "\n" << "File: " << file << "\n";
	SDL_Quit();
	exit(0);
}

SDL_Texture* get_font_texture(const char* message, int font_size, SDL_Rect* message_rect, SDL_Renderer* window_renderer){

    TTF_Font* Roboto = TTF_OpenFont("gfx/Roboto-Regular.ttf", font_size); //this opens a font style and sets a size
	if(!Roboto) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    SDL_Color White = {255, 255, 255};
    SDL_Surface* message_surface = TTF_RenderText_Solid(Roboto, message, White);
    SDL_Texture* message_texture = SDL_CreateTextureFromSurface(window_renderer, message_surface);
    message_rect->w = message_surface->w; // controls the width of the rect
    message_rect->h = message_surface->h; // controls the height of the rect
    SDL_FreeSurface(message_surface);
    return message_texture;
}

SDL_Surface* load_png(const char* path) {
    SDL_Surface* temp = nullptr;
    SDL_Surface* to_return = nullptr;

    temp = IMG_Load(path);
    assert(temp != 0, SDL_GetError());
    to_return = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);

    SDL_FreeSurface(temp);

    return to_return;
}
