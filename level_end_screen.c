#include "engine.h"

level_end_screen *level_end_screen_init(level_end_screen *this)
{
    this->intro_font = TTF_OpenFont(TIMER_TEXT_FONT, TIMER_TEXT_SIZE);

    SDL_Color fg = {0,0,0,255};
    SDL_Surface *temp_surface = TTF_RenderText_Blended(this->intro_font, "Race complete! Your time was:", fg);
    this->intro_text = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);

    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(this->intro_text, NULL, NULL, &this->intro_rect.w, &this->intro_rect.h);
    this->intro_rect.x = (eng.w - this->intro_rect.w)/2;
    this->intro_rect.y = 0.3 * eng.h;

    this->time_font = TTF_OpenFont(TIMER_TEXT_FONT, END_TIME_TEXT_SIZE);
    level_end_screen_time_refresh(this);
    return this;
}

level_end_screen *level_end_screen_render(level_end_screen *this)
{
    SDL_RenderCopy(eng.renderer, this->intro_text, NULL, &this->intro_rect);
    SDL_RenderCopy(eng.renderer, this->time_text, NULL, &this->time_rect);
    return this;
}

level_end_screen *level_end_screen_time_refresh(level_end_screen *this)
{
    SDL_Color fg = {0,0,0,255};

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
    this->time_rect.y = this->intro_rect.y + this->intro_rect.h;

    return this;
}
