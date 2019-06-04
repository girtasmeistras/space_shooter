#include "application.h"
#include <iostream>
#include <string>
#include <fstream>

int Application::level = 1;

Application::Application(): score_str("SCORE: "), offset(0), score(0)
{

    //Initialize SDL
    assert( (SDL_Init(SDL_INIT_VIDEO) > -1), SDL_GetError());

    //Initialize SDL_TTF
    assert( (TTF_Init() > -1), TTF_GetError());

    //Initialize SDL_mixer
    assert( (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) != -1), Mix_GetError() );

    //Create window
    deleted_unique_ptr<SDL_Window> window_temp (SDL_CreateWindow("STL: Slower Than Light",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0), [](SDL_Window* w){SDL_DestroyWindow(w);});
    window = std::move(window_temp);
    assert(window, SDL_GetError());

    //Create window renderer
    deleted_unique_ptr<SDL_Renderer> window_renderer_temp (SDL_CreateRenderer(window.get(),
        -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), [](SDL_Renderer* r) {SDL_DestroyRenderer(r);});
    window_renderer = std::move(window_renderer_temp);
    assert(window_renderer, SDL_GetError());

    //Load music
    deleted_unique_ptr<Mix_Music> m(Mix_LoadMUS("sfx/start.wav"), [](Mix_Music* m) { Mix_FreeMusic(m); });
    music = std::move(m);
    assert(music, Mix_GetError());
    deleted_unique_ptr<Mix_Chunk> c(Mix_LoadWAV("sfx/ok.wav"), [](Mix_Chunk* c) { Mix_FreeChunk(c); });
    ok = std::move(c);
    assert(ok, Mix_GetError());

    //initialize texture manager
    texture_man.load(window_renderer.get());

    //init score texture
    score_count_str = std::to_string(score);
    deleted_unique_ptr<SDL_Texture> temp_score(Helpers::get_font_texture((score_str + score_count_str).c_str(), SCORE_FONT_SIZE, &score_rect, window_renderer.get()),
        [](SDL_Texture* t){SDL_DestroyTexture(t);});
    score_texture = std::move(temp_score);

    background_texture = texture_man.get_texture("background");
    health_texture = texture_man.get_texture("health");
    explosion = texture_man.get_texture("explosion");
    asteroid_texture = texture_man.get_texture("asteroid");
    rocket_texture = texture_man.get_texture("rocket");

    player = std::make_shared<Player>();
    player->load(texture_man.get_texture("player"));
    player->load_bullets(texture_man.get_texture("player_bullet"));
    player->load_animation(explosion);
    player->load_nuke(texture_man.get_texture("rocket"));
    Entity::entity_list.push_back(player);
    enemies.load_bullets(texture_man.get_texture("enemy_bullet"));
    enemies.load_enemies(texture_man.get_texture("enemy"));
    enemies.load_boss(texture_man.get_texture("boss"));
    enemies.load_animations(explosion);
	window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
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
    Entity::entity_list.clear();
	SDL_Quit();
}

bool Application::start()
{
    changed_level = false;
    level = 1;
    Mix_PlayMusic( music.get(), -1 );
    player->restart();
    next_level();
    new_score = 0;
	bool keep_window_open = true;
	SDL_Rect message_rect;
	deleted_unique_ptr<SDL_Texture> start_message(Helpers::get_font_texture("PRESS ENTER TO START", MESSAGE_FONT_SIZE, &message_rect, window_renderer.get()),
        [](SDL_Texture* t){SDL_DestroyTexture(t);});
    message_rect.x = (SCREEN_WIDTH - message_rect.w) / 2;
    message_rect.y = (SCREEN_HEIGHT - message_rect.h) / 2;

	while (keep_window_open)
	{
    SDL_RenderClear(window_renderer.get());
    draw_background();
    SDL_RenderCopy(window_renderer.get(), start_message.get(), NULL, &message_rect);
    SDL_RenderPresent(window_renderer.get());
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				return false;
				break;
            case SDL_KEYDOWN:
                keep_window_open = false;
				break;
			}
		}
        SDL_Delay(1000 / SCREEN_FPS);
	}
	Mix_PlayChannel(-1, ok.get(), 0);
	return true;
}

void Application::game_over()
{
    enemies.restart();
    for(auto it = Entity::entity_list.begin(); it != Entity::entity_list.end(); ++it) {
        (*it)->kill();
    }
    bool keep_window_open = true;
    bool exit = false;
	SDL_Rect message_rect;
	SDL_Texture* end_message = Helpers::get_font_texture("GAME OVER", MESSAGE_FONT_SIZE, &message_rect, window_renderer.get());
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
	SDL_Texture* score_message = Helpers::get_font_texture(("HIGH SCORE: " + std::to_string(stored_score)).c_str(), MESSAGE_FONT_SIZE, &score_rect, window_renderer.get());
    score_rect.x = (SCREEN_WIDTH - score_rect.w) / 2;
    score_rect.y = message_rect.y + 2*message_rect.h;

	while (keep_window_open)
	{
    SDL_RenderClear(window_renderer.get());
    draw_background();
    SDL_RenderCopy(window_renderer.get(), end_message, NULL, &message_rect);
    SDL_RenderCopy(window_renderer.get(), score_message, NULL, &score_rect);
    SDL_RenderPresent(window_renderer.get());
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

	if(exit == false && start()){
        loop();
	}
}

void Application::loop()
{
	bool keep_window_open = true;

	while (keep_window_open)
	{
	    SDL_Rect p_pos_start = player->get_pos();
		while (SDL_PollEvent(&event) > 0)
		{
			player->handle_events(event);
			switch (event.type)
			{
			case SDL_QUIT:
				keep_window_open = false;
				break;
			}
		}
		update();
		draw();
        if(Enemies::wave % 5 == 0 && !changed_level){
            changed_level = true;
            next_level();
		}
        if(Enemies::wave % 5 != 0){
            changed_level = false;
        }
		SDL_Rect p_pos_end = player->get_pos();
        if(p_pos_end.y < p_pos_start.y){
            offset += (p_pos_start.y - p_pos_end.y) + ((p_pos_start.y - p_pos_end.y)/2);
        }
        if(p_pos_end.y > p_pos_start.y){
            offset -= p_pos_start.y - p_pos_end.y;
        }
        if(health <= 0 && !player->is_playing_animation() ){
            break;
        }

	}
    game_over();
}

void Application::next_level(){

    if(changed_level){
    level++;
    }
    if(level > 3){
        level = 1;
    }
    switch(level){
    case 2:
        enemies.load_enemies(texture_man.get_texture("enemy2"));
        background_texture = texture_man.get_texture("background2");
        break;
    case 3:
        enemies.load_enemies(texture_man.get_texture("enemy3"));
        background_texture = texture_man.get_texture("background3");
        break;
    default:
        enemies.load_enemies(texture_man.get_texture("enemy"));
        background_texture = texture_man.get_texture("background");
        break;
    }
}

void Application::update()
{
	player->update();
	enemies.update(player->get_pos());
	check_for_deaths();
	check_for_collisions();
}

void Application::check_for_deaths()
{
    Entity::entity_list.erase(std::remove_if(Entity::entity_list.begin(), Entity::entity_list.end(), completely_dead()), Entity::entity_list.end());
    new_score += level * 100 * std::count_if(Entity::entity_list.begin(), Entity::entity_list.end(), can_get_score());
}

void Application::check_for_collisions()
{
    for(auto it = Entity_Collision::entity_collision_list.begin(); it != Entity_Collision::entity_collision_list.end(); ++it) {
        Entity* a = (*it).first_entity;
        Entity* b = (*it).second_entity;
        if(a == nullptr || b == nullptr)
            continue;
        if(a->is_alive() == false || b->is_alive() == false)
            continue;
        if(a->is_powerup() && !b->is_player())
            continue;
        if(b->is_powerup() && !a->is_player())
            continue;
        a->on_collision(b);
        b->on_collision(a);
    }
    Entity_Collision::entity_collision_list.clear();
}

void Application::draw()
{
	SDL_RenderClear(window_renderer.get());
	draw_background();
	draw_score();
    draw_health();
    draw_entities();
	SDL_RenderPresent(window_renderer.get());
	SDL_Delay(1000 / SCREEN_FPS);
}

void Application::draw_entities()
{
    for(unsigned int i = 0; i < Entity::entity_list.size();i++) {
        if(Entity::entity_list[i]->is_playing_animation()){
            Entity::entity_list[i]->draw_animation(window_renderer.get(), &window_rect);
        }
        else{
            Entity::entity_list[i]->draw(window_renderer.get(), &window_rect);
        }
    }
}

void Application::draw_health()
{
    health = player->get_health();
    if(Enemies::wave % SPAWN_RATE == 0 && Asteroid::count == 0 && !spawned_asteroid){
        auto asteroid = std::make_shared<Asteroid>();
        asteroid->load(asteroid_texture);
        asteroid->load_animation(explosion);
        asteroid->load_powerups(health_texture, rocket_texture);
        Entity::entity_list.push_back(std::move(asteroid));
        spawned_asteroid = true;
    }
    if(Enemies::wave % SPAWN_RATE != 0){
        spawned_asteroid = false;
    }
    for(int i = 0; i < health && i < health_rects.size(); i++){
        health_texture.draw(window_renderer.get(), &window_rect, &health_rects[i]);
    }
}

void Application::draw_background()
{
    //draws scrolling background on screen
    ++offset;
    if( offset > SCREEN_HEIGHT ){
        offset = 0;
    }
    background_texture.clipped_render(0, offset, 0, window_renderer.get());
    background_texture.clipped_render(0, offset - SCREEN_HEIGHT, 0, window_renderer.get());
}

void Application::draw_score()
{
    if(score != new_score){
        score = new_score;
        score_count_str = std::to_string(score);
        deleted_unique_ptr<SDL_Texture> temp_score(Helpers::get_font_texture((score_str + score_count_str).c_str(), SCORE_FONT_SIZE, &score_rect, window_renderer.get()),
            [](SDL_Texture* t){SDL_DestroyTexture(t);});
        score_texture.release();
        score_texture = std::move(temp_score);
    }
    SDL_RenderCopy(window_renderer.get(), score_texture.get(), &window_rect, &score_rect);
}
