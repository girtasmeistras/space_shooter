#include "application.h"
#include <iostream>
#include <string.h>

Application::Application()
{
	//Initialize SDL
	assert( (SDL_Init(SDL_INIT_VIDEO) > -1), SDL_GetError());

	//initialize SDL_TTF
    assert( (TTF_Init() > -1), TTF_GetError());

	window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	assert( (window != 0), SDL_GetError());

	window_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert( (window_renderer != 0), SDL_GetError());

	background_texture.load_texture("gfx/background.png", window_renderer);
	health_texture.load_texture("gfx/health.png", window_renderer);
	player.load_bullets("gfx/p_bullet.png", window_renderer);
	enemies.load_bullets("gfx/e_bullet.png", window_renderer);
	player.load("gfx/playership.png", window_renderer);
	enemies.load_enemies("gfx/enemyship.png", window_renderer);
    enemies.load_animations("gfx/explosion.png", window_renderer);
	Entity::entity_list.push_back(&player);

	window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    offset = 0;
    score = 0;
    new_score = 0;
    score_str = "SCORE: ";
    score_count_str = std::to_string(score);
    score_texture = get_font_texture((score_str + score_count_str).c_str(), SCORE_FONT_SIZE, &score_rect, window_renderer);
    score_rect.x = (SCREEN_WIDTH - score_rect.w) / 2;
    score_rect.y = SCREEN_HEIGHT - score_rect.h;
    health_rect.w = 30;
    health_rect.h = 31;
    health_rect.x = 0;
    health_rect.y = 0;

}

Application::~Application()
{

    for(int i = 0;i < Entity::entity_list.size();i++) {
        if(Entity::entity_list[i] == nullptr) continue;
            Entity::entity_list[i]->clean_up();
    }
    Entity::entity_list.clear();

	SDL_DestroyRenderer(window_renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int Application::start()
{
	bool keep_window_open = true;
	SDL_Rect message_rect;
	SDL_Texture* start_message = get_font_texture("PRESS ENTER TO START", 24, &message_rect, window_renderer);
    message_rect.x = (SCREEN_WIDTH - message_rect.w) / 2;
    message_rect.y = (SCREEN_HEIGHT - message_rect.h) / 2;

	while (keep_window_open)
	{

    offset++;
    if( offset > SCREEN_HEIGHT )
    {
        offset = 0;
    }
    SDL_RenderClear(window_renderer);
    background_texture.clipped_render(0, offset, 0, window_renderer);
    background_texture.clipped_render(0, offset - SCREEN_HEIGHT, 0, window_renderer);
    SDL_RenderCopy(window_renderer, start_message, NULL, &message_rect);
    SDL_RenderPresent(window_renderer);
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				return 0;
				break;
            case SDL_KEYDOWN:
                keep_window_open = false;
				break;
			}
		}
	}
	SDL_DestroyTexture(start_message);
	return 1;
}


void Application::loop()
{
	bool keep_window_open = true;
	while (keep_window_open)
	{
	    SDL_Rect p_pos_start = player.get_pos();

		while (SDL_PollEvent(&event) > 0)
		{
			player.handle_events(event);

			switch (event.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				break;
			}
		}

		update();
		draw();
		SDL_Rect p_pos_end = player.get_pos();
        if(p_pos_end.y < p_pos_start.y){
            offset += p_pos_start.y - p_pos_end.y;
        }
	}
}



void Application::update()
{

    ++offset;
    if( offset > SCREEN_HEIGHT )
    {
        offset = 0;
    }

	player.update();

	enemies.update(player.get_pos());

    for(int i = 0;i < Entity::entity_list.size();i++) {
        if(Entity::entity_list[i]->is_alive() == false){

            if(Entity::entity_list[i]->get_flag() == 3 && Entity::entity_list[i]->was_already_probed() <= 1){

                new_score += 100;
            }

            if(Entity::entity_list[i]->is_playing_animation() == false){
                delete Entity::entity_list[i];
                Entity::entity_list.erase(Entity::entity_list.begin()+i);
            }
        }
    }

    for(int i = 0; i < Entity_Collision::entity_collision_list.size(); i++) {
        Entity* a = Entity_Collision::entity_collision_list[i].first_entity;
        Entity* b = Entity_Collision::entity_collision_list[i].second_entity;

        if(a == nullptr || b == nullptr){
            continue;
        }

        if(a->is_alive() == false || b->is_alive() == false){
            continue;
        }

        a->on_collision(b);
        b->on_collision(a);

    }
    Entity_Collision::entity_collision_list.clear();
}

void Application::draw()
{
    if(score < new_score){
        score = new_score;
        score_count_str = std::to_string(score);
        score_texture = get_font_texture( (score_str + score_count_str).c_str(), SCORE_FONT_SIZE, &score_rect, window_renderer);
    }

    int health = player.get_health() / 25;
    SDL_Rect health_rect_temp = health_rect;

	SDL_RenderClear(window_renderer);

	//background_texture.draw(window_renderer, &window_rect, nullptr);
    background_texture.clipped_render(0, offset, 0, window_renderer);
    background_texture.clipped_render(0, offset - SCREEN_HEIGHT, 0, window_renderer);
    SDL_RenderCopy(window_renderer, score_texture, NULL, &score_rect);

    for(int i = 0; i < health; i++){
        health_rect_temp.x += health_rect.x;
        health_texture.draw(window_renderer, &window_rect, &health_rect_temp);
    }

	for(unsigned int i = 0;i < Entity::entity_list.size();i++) {

        if(Entity::entity_list[i]->is_playing_animation() == true){
            Entity::entity_list[i]->draw_animation(window_renderer, &window_rect);

        }else{
            Entity::entity_list[i]->draw(window_renderer, &window_rect);
        }
    }

	SDL_RenderPresent(window_renderer);


	SDL_Delay(1000 / SCREEN_FPS);
}

