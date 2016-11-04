#include "engine.h"

level_intro_screen *level_intro_screen_init(level_intro_screen *this)
{
    this->heading_font = TTF_OpenFont(TIMER_TEXT_FONT, END_TIME_TEXT_SIZE);
    this->body_font = TTF_OpenFont(TIMER_TEXT_FONT, TIMER_TEXT_SIZE);

    this->heading_text = NULL;
    this->body_text = NULL;

    level_intro_screen_refresh_text(this);
    
    return this;
}

level_intro_screen *level_intro_screen_refresh_text_sp(level_intro_screen *this)
{
    SDL_Color fg = {0,0,0,255};
    SDL_Surface *temp_surface = TTF_RenderText_Blended(this->heading_font, "Ready? Press Enter!", fg);
    this->heading_text = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->heading_text, NULL, NULL, &this->heading_rect.w, &this->heading_rect.h);

    this->heading_rect.x = (eng.w - this->heading_rect.w)/2;
    this->heading_rect.y = 0.25 * eng.h;

    char msg[256];
    sprintf(msg, "Target time: %.2f s", (double)eng.target_time_ms / 1000);
    temp_surface = TTF_RenderText_Blended(this->body_font, msg, fg);
    this->body_text = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->body_text, NULL, NULL, &this->body_rect.w, &this->body_rect.h);

    this->body_rect.x = (eng.w - this->body_rect.w)/2;
    this->body_rect.y = this->heading_rect.y + this->heading_rect.h;

    return this;
}

level_intro_screen *level_intro_screen_refresh_text_2p(level_intro_screen *this)
{
    char msg[256];
    SDL_Color fg = {0,0,0,255};

    sprintf(msg, "Player %d's turn", eng.active_player);
    SDL_Surface *temp_surface = TTF_RenderText_Blended(this->heading_font, msg, fg);

    this->heading_text = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->heading_text, NULL, NULL, &this->heading_rect.w, &this->heading_rect.h);

    this->heading_rect.x = (eng.w - this->heading_rect.w)/2;
    this->heading_rect.y = 0.25 * eng.h;

    temp_surface = TTF_RenderText_Blended(this->body_font, "Press enter to start, press space when done", fg);
    this->body_text = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->body_text, NULL, NULL, &this->body_rect.w, &this->body_rect.h);

    this->body_rect.x = (eng.w - this->body_rect.w)/2;
    this->body_rect.y = this->heading_rect.y + this->heading_rect.h;

    return this;
    
}

level_intro_screen *level_intro_screen_refresh_text(level_intro_screen *this)
{
    if (eng.play_mode == PLAY_MODE_SP)
        level_intro_screen_refresh_text_sp(this);
    else
        level_intro_screen_refresh_text_2p(this);
    return this;
}

level_intro_screen *level_intro_screen_render(level_intro_screen *this)
{
    SDL_RenderCopy(eng.renderer, this->heading_text, NULL, &this->heading_rect);
    SDL_RenderCopy(eng.renderer, this->body_text, NULL, &this->body_rect);
    return this;
}
