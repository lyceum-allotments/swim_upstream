#include "engine.h"

void level_end_screen_create_intro_text(level_end_screen *this, SDL_Texture **target_texure, SDL_Rect *target_rect, const char *string)
{
    SDL_Color fg = {0,0,0,255};
    SDL_Surface *temp_surface = TTF_RenderText_Blended(this->intro_font, string, fg);
    *target_texure = SDL_CreateTextureFromSurface(eng.renderer, temp_surface);

    SDL_FreeSurface(temp_surface);

    SDL_QueryTexture(*target_texure, NULL, NULL, &target_rect->w, &target_rect->h);
    target_rect->x = (eng.w - target_rect->w)/2;
    target_rect->y = 0.3 * eng.h;
}

level_end_screen *level_end_screen_init(level_end_screen *this)
{
    if (eng.play_mode == PLAY_MODE_SP)
    {
        this->intro_font = TTF_OpenFont(TIMER_TEXT_FONT, TIMER_TEXT_SIZE);
        this->time_font = TTF_OpenFont(TIMER_TEXT_FONT, END_TIME_TEXT_SIZE);
    }
    else
    {
        this->intro_font = TTF_OpenFont(TIMER_TEXT_FONT, END_TIME_TEXT_SIZE);
        this->time_font = TTF_OpenFont(TIMER_TEXT_FONT, TIMER_TEXT_SIZE);
    }

    level_end_screen_create_intro_text(this, &this->intro_text_success, &this->intro_rect_success, "Challenge complete! Your time was:");
    level_end_screen_create_intro_text(this, &this->intro_text_failed, &this->intro_rect_failed, "Challenge failed! Your time was:");

    level_end_screen_create_intro_text(this, &this->intro_text_p1_win, &this->intro_rect_p1_win, "P1 wins!");
    level_end_screen_create_intro_text(this, &this->intro_text_p2_win, &this->intro_rect_p2_win, "P2 wins!");

    this->intro_rect = &this->intro_rect_success;

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

    if (eng.play_mode == PLAY_MODE_SP)
    {
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
    }
    else if (eng.active_states[GAME_STATE_LEVEL_FINISHED])
    {
        if (eng.fish_actor[0].finish_frames < eng.fish_actor[1].finish_frames)
        {
            this->intro_text = this->intro_text_p1_win;
            this->intro_rect = &this->intro_rect_p1_win;
        }
        else
        {
            this->intro_text = this->intro_text_p2_win;
            this->intro_rect = &this->intro_rect_p2_win;
        }
    }

    char msg[201];
    if (eng.play_mode == PLAY_MODE_SP)
        sprintf(msg, "%.2f", (double)eng.frames_swimming/(double)eng.fps);
    else 
        sprintf(msg, "P1: %.2f P2: %.2f", (double)eng.fish_actor[0].finish_frames/(double)eng.fps,
                                          (double)eng.fish_actor[1].finish_frames/(double)eng.fps);

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
