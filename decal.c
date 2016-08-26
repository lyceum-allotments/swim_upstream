#include "engine.h"
#include "decal.h"

decal *decal_init(
        decal *d,
        SDL_Texture *texture,
        double x,
        double y,
        double w,
        double h
)
{
    d->texture = texture;
    decal_set_xywh(d, x, y, w, h);
    return d;
}

void decal_set_xywh(
        decal *d,
        double x,
        double y,
        double w,
        double h
)
{
    d->rect.x = x;
    d->rect.y = y;
    d->rect.w = w;
    d->rect.h = h;
}
