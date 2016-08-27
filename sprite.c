#include "engine.h"
#include "sprite.h"

sprite *sprite_init(
        sprite *sp,
        int w,
        int h,
        decal *d)
{
    sp->w = w;
    sp->h = h;
    sp->d = d;
    sp->r[0] = 0.0f;
    sp->r[1] = 0.0f;

    sp->rot_angle = 0;
    sp->rot_centre[0] = w / 2;
    sp->rot_centre[1] = h / 2;

    return sp;
}

void sprite_render(sprite *sp)
{
    SDL_Rect dest;
    dest.x = sp->r[0];
    dest.y = sp->r[1];
    dest.w = sp->w;
    dest.h = sp->h;

    SDL_Point centre = {.x = sp->rot_centre[0], .y = sp->rot_centre[1]};

    SDL_RenderCopyEx(eng.renderer, sp->d->texture, &sp->d->rect, &dest, sp->rot_angle, &centre, SDL_FLIP_NONE);
}

void sprite_set_decal(sprite *sp, decal *d)
{
    sp->d = d;
}
