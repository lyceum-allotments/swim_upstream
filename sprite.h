#ifndef SPRITE_H
#define SPRITE_H

#include "decal.h"

typedef struct sprite sprite;

struct sprite
{
    int w;
    int h;
    int r[2];

    decal *d;
};

sprite *sprite_init(
        sprite *sp,
        int w,
        int h,
        decal *d);

void sprite_destroy(sprite *sp);

void sprite_render(sprite *sp);

void sprite_set_decal(sprite *sp, decal *d);

#endif
