#pragma once

#define assert(expression, message) if(expression == false) Helpers::assert_func(message, __LINE__, __func__, __FILE__)
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const float DELTA_TIME = 0.016;

#include "SDL2/SDL.h"
#include<SDL2/SDL_image.h>
#include "SDL2/SDL_ttf.h"
#include <array>
#include <unordered_map>
#include <memory>
#include <sstream>

template<typename T>
using deleted_unique_ptr = std::unique_ptr<T,std::function<void(T*)>>;

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

class Helpers{
public:
    Helpers()=default;
    ~Helpers()=default;
    static SDL_Surface* load_bmp(const char* path);
    static SDL_Surface* load_png(const char* path);
    static SDL_Texture* get_font_texture(const char* message, int font_size, SDL_Rect* message_rect, SDL_Renderer* window_renderer);
    static void assert_func(char const *message, int line, char const *function, char const *file);
};

class Assertion_Failed_Exception: public std::runtime_error{
public:
    Assertion_Failed_Exception(const char* message): std::runtime_error(message) {}
};


class Texture_Manager{
public:
    Texture_Manager() = default;
    ~Texture_Manager();
    void load(SDL_Renderer* window_renderer);
    const Image & get_texture(const std::string & id) const;
private:
    std::unordered_map<std::string, Image> texture_map;
    Image empty_texture;
};

template <typename T>
class vec2d {
private:
  T x, y;

public:
  vec2d(): x(0), y(0) { }
  vec2d(const T& x, const T& y): x(x), y(y) { }

  const T& get_x() const { return x; }
  const T& get_y() const { return y; }

  void set_x(T new_x){ x = new_x; }
  void set_y(T new_y) { y = new_y; }
  template <typename R>
  vec2d<T> operator + (const vec2d<R>& right) const {
    return vec2d<T>(x + right.get_x(), y + right.get_y());
  }

  template <typename R>
  vec2d<T>& operator += (const vec2d<R>& right) {
    x += right.get_x();
    y += right.get_y();
    return *this;
  }

   template <typename R>
  vec2d<T>& operator = (const vec2d<R>& right) {
    x = right.get_x();
    y = right.get_y();
    return *this;
  }

  template <typename R>
  vec2d<T> operator * (const R& right) const {
    return vec2d<T>(x * right, y * right);
  }

  template <typename R>
  vec2d<T>& operator *= (const R& right) {
    x *= right;
    y *= right;
    return *this;
  }

  vec2d<T>& operator < (const T& right) {
    x += right;
    return *this;
  }

  vec2d<T>& operator > (const T& right) {
    y += right;
    return *this;
  }

  vec2d<T>& operator << (const T& right) {
    x -= right;
    return *this;
  }

  vec2d<T>& operator >> (const T& right) {
    y -= right;
    return *this;
  }
};

template <typename T, typename R>
vec2d<T>& operator *= (vec2d<T> & left, const R& right) {
    left *= right;
    return left;
}

template <typename T, typename R>
vec2d<T> operator * (const vec2d<T> & left, const R& right){
    return left*right;
}

template <typename T, typename R>
  vec2d<T>& operator += (vec2d<T>& left, const vec2d<R>& right) {
    left += right;
    return left;
}

template <typename T, typename R>
vec2d<T> operator + (const vec2d<T>& left, const vec2d<R>& right){
    return left+right;
}

template <typename T>
vec2d<T>& operator < (vec2d<T>& left, const T& right) {
    left < right;
    return left;
  }
template <typename T>
vec2d<T>& operator > (vec2d<T>& left, const T& right) {
    left > right;
    return left;
  }

template <typename T>
vec2d<T>& operator << (vec2d<T>& left, const T& right) {
    left << right;
    return left;
  }
template <typename T>
vec2d<T>& operator >> (vec2d<T>& left, const T& right) {
    left >> right;
    return left;
  }
