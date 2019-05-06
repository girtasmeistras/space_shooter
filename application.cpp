#include "application.h"
#include <iostream>
#include <string>
#include <fstream>

Application::Application(): score_str("SCORE: ")
{
	//Initialize SDL
	assert( (SDL_Init(SDL_INIT_VIDEO) > -1), SDL_GetError());

	//Initialize SDL_TTF
    assert( (TTF_Init() > -1), TTF_GetError());

	window = SDL_CreateWindow("STL: Slower Than Light", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	assert( (window != 0), SDL_GetError());

	window_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert( (window_renderer != 0), SDL_GetError());

	background_texture.load_texture("gfx/background.png", window_renderer);
	health_texture.load_texture("gfx/health.png", window_renderer);
	explosion.load_texture("gfx/explosion.png", window_renderer);
	player.load("gfx/playership.png", window_renderer);
	player.load_bullets("gfx/p_bullet.png", window_renderer);
	player.load_animation(explosion);
	player.load_nuke("gfx/rocket.png", window_renderer);
	enemies.load_bullets("gfx/e_bullet.png", window_renderer);
	enemies.load_enemies("gfx/enemy_other.png", window_renderer);
    enemies.load_animations(explosion);
    asteroid_texture.load_texture("gfx/asteroid.png", window_renderer);
    rocket_texture.load_texture("gfx/rocket.png", window_renderer);
	Entity::entity_list.push_back(&player);
	window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    offset = 0;
    score = 0;
    score_count_str = std::to_string(score);
    score_texture = Helpers::get_font_texture((score_str + score_count_str).c_str(), SCORE_FONT_SIZE, &score_rect, window_renderer);
    score_rect.x = (SCREEN_WIDTH - score_rect.w) / 2;
    score_rect.y = SCREEN_HEIGHT - score_rect.h;

    SDL_QueryTexture(health_texture.get_texture(), 0, 0, &health_rects[0].w, &health_rects[0].h);
    health_rects[0].x = 5;//offset x,y from corner
    health_rects[0].y = 5;
    for(int i = 1; i < 4; i++){
        health_rects[i] = health_rects[0];
        health_rects[i].x += i*health_rects[i].w;
    }
}

Application::~Application()
{
     for(unsigned int i = 1;i < Entity::entity_list.size();i++) {
        delete Entity::entity_list[i];
        Entity::entity_list.erase(Entity::entity_list.begin()+i);
     }
    SDL_DestroyTexture(score_texture);
	SDL_DestroyRenderer(window_renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int Application::start()
{
    player.restart();
    new_score = 0;
	bool keep_window_open = true;
	SDL_Rect message_rect;
	SDL_Texture* start_message = Helpers::get_font_texture("PRESS ENTER TO START", MESSAGE_FONT_SIZE, &message_rect, window_renderer);
    message_rect.x = (SCREEN_WIDTH - message_rect.w) / 2;
    message_rect.y = (SCREEN_HEIGHT - message_rect.h) / 2;

	while (keep_window_open)
	{
    SDL_RenderClear(window_renderer);
    draw_background();
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
        SDL_Delay(1000 / SCREEN_FPS);
	}
	SDL_DestroyTexture(start_message);
	return 1;
}

void Application::game_over()
{
    enemies.restart();
    for(unsigned int i = 0;i < Entity::entity_list.size(); i++) {
        Entity::entity_list[i]->kill();
    }
    bool keep_window_open = true;
    bool exit = false;
	SDL_Rect message_rect;
	SDL_Texture* end_message = Helpers::get_font_texture("GAME OVER", MESSAGE_FONT_SIZE, &message_rect, window_renderer);
    message_rect.x = (SCREEN_WIDTH - message_rect.w) / 2;
    message_rect.y = (SCREEN_HEIGHT - message_rect.h) / 2;

    std::fstream fs;
    int stored_score;
	fs.open("score.txt");
	if(fs){
        fs >> stored_score;
        if(score > stored_score){
            fs.clear();
            fs.seekg(0, std::ios::beg);
            fs << score;
            stored_score = score;
        }
	}
	else{
        fs.open("score.txt", std::fstream::out);
        fs << score;
        stored_score = score;
	}
	fs.close();

    SDL_Rect score_rect;
	SDL_Texture* score_message = Helpers::get_font_texture(("HIGH SCORE: " + std::to_string(stored_score)).c_str(), MESSAGE_FONT_SIZE, &score_rect, window_renderer);
    score_rect.x = (SCREEN_WIDTH - score_rect.w) / 2;
    score_rect.y = message_rect.y + 2*message_rect.h;

	while (keep_window_open)
	{
    SDL_RenderClear(window_renderer);
    draw_background();
    SDL_RenderCopy(window_renderer, end_message, NULL, &message_rect);
    SDL_RenderCopy(window_renderer, score_message, NULL, &score_rect);
    SDL_RenderPresent(window_renderer);
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				exit = true;
				break;
            case SDL_KEYDOWN:

            switch( event.key.keysym.sym )
                {
                case SDLK_RETURN:
                keep_window_open = false;
				break;
                }
			}
		}
		SDL_Delay(1000 / SCREEN_FPS);
	}
	SDL_DestroyTexture(end_message);

	if(exit == false && start() == 1){
        loop();
	}
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
            offset += (p_pos_start.y - p_pos_end.y) + ((p_pos_start.y - p_pos_end.y)/2);
        }
        if(p_pos_end.y > p_pos_start.y){
            offset -= p_pos_start.y - p_pos_end.y;
        }
        if(health <= 0 && !player.is_playing_animation() ){
            break;
        }
	}
    game_over();
}



void Application::update()
{
	player.update();
	enemies.update(player.get_pos());
	check_for_deaths();
	check_for_collisions();
}

void Application::check_for_deaths()
{
    for(unsigned int i = 1;i < Entity::entity_list.size();i++) {
        if(Entity::entity_list[i]->is_alive() == false){
            if(dynamic_cast<Enemy*>(Entity::entity_list[i]) && Entity::entity_list[i]->already_got_score() == false){
                new_score += 100;
            }
            if(Entity::entity_list[i]->is_playing_animation() == false){
                delete Entity::entity_list[i];
                Entity::entity_list.erase(Entity::entity_list.begin()+i);
            }
        }
    }
}

void Application::check_for_collisions()
{
    for(unsigned int i = 0; i < Entity_Collision::entity_collision_list.size(); i++) {
        Entity* a = Entity_Collision::entity_collision_list[i].first_entity;
        Entity* b = Entity_Collision::entity_collision_list[i].second_entity;
        if(a == nullptr || b == nullptr)
            continue;
        if(a->is_alive() == false || b->is_alive() == false)
            continue;

        a->on_collision(b);
        b->on_collision(a);
    }
    Entity_Collision::entity_collision_list.clear();
}

void Application::draw()
{
	SDL_RenderClear(window_renderer);
	draw_background();
	draw_score();
    draw_health();
    draw_entities();
	SDL_RenderPresent(window_renderer);
	SDL_Delay(1000 / SCREEN_FPS);
}

void Application::draw_entities()
{
    for(unsigned int i = 0;i < Entity::entity_list.size();i++) {
        if(Entity::entity_list[i]->is_playing_animation() == true){
            Entity::entity_list[i]->draw_animation(window_renderer, &window_rect);
        }
        else{
            Entity::entity_list[i]->draw(window_renderer, &window_rect);
        }
    }
}

void Application::draw_health()
{
    health = player.get_health();
    if(Enemies::wave % SPAWN_RATE == 0 && Asteroid::count == 0 && !spawned_asteroid){
        Asteroid* asteroid = new Asteroid();
        asteroid->load(asteroid_texture);
        asteroid->load_animation(explosion);
        asteroid->load_powerups(health_texture, rocket_texture);
        Entity::entity_list.push_back(asteroid);
        spawned_asteroid = true;
    }
    if(Enemies::wave % SPAWN_RATE != 0){
        spawned_asteroid = false;
    }
    for(int i = 0; i < health && i < health_rects.size(); i++){
        health_texture.draw(window_renderer, &window_rect, &health_rects[i]);
    }
}

void Application::draw_background()
{
    //draws scrolling background on screen
    ++offset;
    if( offset > SCREEN_HEIGHT ){
        offset = 0;
    }
    background_texture.clipped_render(0, offset, 0, window_renderer);
    background_texture.clipped_render(0, offset - SCREEN_HEIGHT, 0, window_renderer);
}

void Application::draw_score()
{
    if(score != new_score){
        score = new_score;
        score_count_str = std::to_string(score);
        score_texture = Helpers::get_font_texture((score_str + score_count_str).c_str(), SCORE_FONT_SIZE, &score_rect, window_renderer);
    }
    SDL_RenderCopy(window_renderer, score_texture, &window_rect, &score_rect);
}
