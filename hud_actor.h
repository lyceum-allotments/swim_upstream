#ifndef HUD_ACTOR_H
#define HUD_ACTOR_H
#include "engine.h"

typedef struct hud_actor hud_actor;

struct hud_actor {
    actor a;

    TTF_Font *font;
    SDL_Texture *timer_text;
};

hud_actor *hud_actor_init(hud_actor *this);
hud_actor *hud_actor_update_timer(hud_actor *this);
#endif
