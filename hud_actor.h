#ifndef HUD_ACTOR_H
#define HUD_ACTOR_H
#include "engine.h"
#include "level_end_screen.h"

typedef struct hud_actor hud_actor;

struct hud_actor {
    actor a;

    TTF_Font *timer_font;
    SDL_Texture *timer_text;

    level_end_screen level_end_screen;
};

hud_actor *hud_actor_init(hud_actor *this);
hud_actor *hud_actor_update_timer(hud_actor *this);
#endif
