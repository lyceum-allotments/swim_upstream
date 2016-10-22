#include "engine.h"

level_end_screen *level_end_screen_init(level_end_screen *this)
{
    this->intro_font = TTF_OpenFont(TIMER_TEXT_FONT, TIMER_TEXT_SIZE);

    SDL_Color fg = {0,0,0,255};
    SDL_Surface *temp_surface = TTF_RenderText_Blended(this->intro_font, "Challenge complete! Your time was:", fg);
    this->intro_text_success = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);

    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->intro_text_success, NULL, NULL, &this->intro_rect_success.w, &this->intro_rect_success.h);
    this->intro_rect_success.x = (eng.w - this->intro_rect_success.w)/2;
    this->intro_rect_success.y = 0.3 * eng.h;


    temp_surface = TTF_RenderText_Blended(this->intro_font, "Challenge failed! Your time was:", fg);
    this->intro_text_failed = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);

    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->intro_text_failed, NULL, NULL, &this->intro_rect_failed.w, &this->intro_rect_failed.h);
    this->intro_rect_failed.x = (eng.w - this->intro_rect_failed.w)/2;
    this->intro_rect_failed.y = 0.3 * eng.h;

    this->time_font = TTF_OpenFont(TIMER_TEXT_FONT, END_TIME_TEXT_SIZE);
    level_end_screen_time_refresh(this);
    return this;
}

level_end_screen *level_end_screen_render(level_end_screen *this)
{
    SDL_RenderCopy(eng.renderer, this->intro_text, NULL, this->intro_rect);
    SDL_RenderCopy(eng.renderer, this->time_text, NULL, &this->time_rect);
    return this;
}

level_end_screen *level_end_screen_time_refresh(level_end_screen *this)
{
    SDL_Color fg = {0,0,0,255};

    if (eng.active_states[GAME_STATE_PASSED_CHALLENGE])
    {
        this->intro_text = this->intro_text_success;
        this->intro_rect = &this->intro_rect_success;
    }
    else
    {
        this->intro_text = this->intro_text_failed;
        this->intro_rect = &this->intro_rect_failed;
    }

    char msg[201];
    sprintf(msg, "%.2f", (double)eng.frames_swimming/(double)eng.fps);
    SDL_Surface *temp_surface = TTF_RenderText_Blended(this->time_font, msg, fg);
    if (this->time_text)
        SDL_DestroyTexture(this->time_text);
    this->time_text = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);

    this->time_rect.w = temp_surface->w;
    this->time_rect.h = temp_surface->h;
    SDL_FreeSurface(temp_surface);

    this->time_rect.x = (eng.w - this->time_rect.w)/2;
    this->time_rect.y = this->intro_rect->y + this->intro_rect->h;

    return this;
}
