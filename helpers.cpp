#include "helpers.h"
#include <iostream>
#include <string>

Image::Image() :
	texture(nullptr) {}

Image::~Image() {}

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

    if( clip != nullptr ){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
	SDL_RenderCopyEx( w_ren, texture, clip, &renderQuad, 0, 0, SDL_FLIP_NONE);
}

SDL_Texture* Image::get_texture() const{

    return texture;
}

void Image::load_texture(const char* path, SDL_Renderer* w_ren) {

	SDL_Surface* temp = Helpers::load_png(path);
	assert((temp != 0), SDL_GetError());
	texture = SDL_CreateTextureFromSurface(w_ren, temp);
	assert((texture != 0), SDL_GetError());
	SDL_FreeSurface(temp);

}

SDL_Surface* Helpers::load_bmp(const char* path)
{
	SDL_Surface *image_surface = SDL_LoadBMP(path);
	assert( (image_surface != 0), SDL_GetError());
	return image_surface;
}


void Helpers::assert_func(char const *message, int line, char const *function, char const *file)
{
    std::stringstream ss;
	ss << "ASSERTION FAILED: \n" << "Message: " << message << "\n" << "Line: " << line << "\n" << "Function: " << function << "\n" << "File: " << file << "\n";
	std::string a = ss.str();
    throw Assertion_Failed_Exception(a.c_str());
}

SDL_Texture* Helpers::get_font_texture(const char* message, int font_size, SDL_Rect* message_rect, SDL_Renderer* window_renderer){

    TTF_Font* Roboto = nullptr;
    Roboto = TTF_OpenFont("gfx/Roboto-Regular.ttf", font_size); //this opens a font style and sets a size
	assert(Roboto, TTF_GetError());
    SDL_Color White = {255, 255, 255};
    SDL_Surface* message_surface = TTF_RenderText_Solid(Roboto, message, White);
    SDL_Texture* message_texture = SDL_CreateTextureFromSurface(window_renderer, message_surface);
    message_rect->w = message_surface->w; // controls the width of the rect
    message_rect->h = message_surface->h; // controls the height of the rect
    SDL_FreeSurface(message_surface);
    return message_texture;
}

SDL_Surface* Helpers::load_png(const char* path) {
    SDL_Surface* temp = nullptr;
    SDL_Surface* to_return = nullptr;

    temp = IMG_Load(path);
    assert( (temp != 0) , IMG_GetError());
    to_return = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(temp);
    return to_return;
}

Texture_Manager::~Texture_Manager(){

    for(auto it: texture_map){
        auto p = it.second;
        p.destroy();
    }

}

void Texture_Manager::load(SDL_Renderer* window_renderer){
    Image background_texture;
    Image background_texture2;
    Image background_texture3;
	Image health_texture;
	Image asteroid_texture;
	Image explosion_texture;
	Image rocket_texture;
	Image player_texture;
	Image player_bullet_texture;
	Image enemy_texture;
	Image enemy_texture2;
	Image enemy_texture3;
	Image enemy_bullet_texture;
	Image boss_texture;

	background_texture.load_texture("gfx/background.png", window_renderer);
	texture_map.insert({"background", background_texture});
	background_texture2.load_texture("gfx/background2.png", window_renderer);
	texture_map.insert({"background2", background_texture2});
	background_texture3.load_texture("gfx/background3.png", window_renderer);
	texture_map.insert({"background3", background_texture3});
	health_texture.load_texture("gfx/health.png", window_renderer);
	texture_map.insert({"health", health_texture});
	asteroid_texture.load_texture("gfx/asteroid.png", window_renderer);
	texture_map.insert({"asteroid", asteroid_texture});
	explosion_texture.load_texture("gfx/explosion.png", window_renderer);
	texture_map.insert({"explosion", explosion_texture});
    rocket_texture.load_texture("gfx/rocket.png", window_renderer);
    texture_map.insert({"rocket", rocket_texture});
	player_texture.load_texture("gfx/playership.png", window_renderer);
	texture_map.insert({"player", player_texture});
	player_bullet_texture.load_texture("gfx/p_bullet.png", window_renderer);
	texture_map.insert({"player_bullet", player_bullet_texture});
	enemy_texture.load_texture("gfx/enemy_other.png", window_renderer);
	texture_map.insert({"enemy", enemy_texture});
	enemy_texture2.load_texture("gfx/enemy_new.png", window_renderer);
	texture_map.insert({"enemy2", enemy_texture2});
	enemy_texture3.load_texture("gfx/enemyship.png", window_renderer);
	texture_map.insert({"enemy3", enemy_texture3});
	enemy_bullet_texture.load_texture("gfx/e_bullet.png", window_renderer);
	texture_map.insert({"enemy_bullet", enemy_bullet_texture});
	boss_texture.load_texture("gfx/boss.png", window_renderer);
    texture_map.insert({"boss", boss_texture});

}

const Image & Texture_Manager::get_texture(const std::string & id) const{

    auto to_find = texture_map.find(id);

    if(to_find != texture_map.end()){
        return to_find->second;
    }else{
        std::cerr << "texture with id " << id << " not found" << std::endl;
        return empty_texture;

    }
}
