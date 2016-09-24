#include "engine.h"

void hud_render_handler(actor *a)
{
    hud_actor *this = (hud_actor *)a;

    SDL_Rect dest;
    SDL_QueryTexture(this->timer_text,
            NULL, NULL, &dest.w, &dest.h);
    dest.x = 40;
    dest.y = eng.h - 5 - dest.h;
    SDL_RenderCopy (eng.renderer, this->timer_text, NULL, &dest);
}

void hud_logic_handler(actor *a)
{
    hud_actor_update_timer((hud_actor *)a);
}

hud_actor *hud_actor_init(hud_actor *this)
{
    actor_init(&this->a, hud_render_handler, hud_logic_handler);
    
    this->font = TTF_OpenFont(TIMER_TEXT_FONT, TIMER_TEXT_SIZE);
    hud_actor_update_timer(this);

    return this;
}

hud_actor *hud_actor_update_timer(hud_actor *this)
{
	SDL_Color fg={0,0,0,255};
    char msg[201];
    sprintf(msg, "%.2f", (double)eng.frames_swimming/(double)eng.fps);
    SDL_Surface *timer_text_surface = TTF_RenderText_Blended(this->font, msg, fg);
    this->timer_text = SDL_CreateTextureFromSurface(eng.renderer, timer_text_surface);
    SDL_FreeSurface(timer_text_surface);
    return this;
}
