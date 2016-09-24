#include "engine.h"

void bg_render_handler(actor *a)
{
    bg_actor *bg = (bg_actor *)a;
    sprite_render(&(bg->sprite));
}

bg_actor *bg_actor_init(bg_actor *bg)
{
    actor_init(&bg->a, bg_render_handler, NULL);
    sprite_init(
            &bg->sprite,
            eng.w,
            eng.h,
            &eng.bg_decal);
    bg->sprite.r[0] = 0;
    bg->sprite.r[1] = 0;
    return bg;
}
