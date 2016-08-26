#ifndef DECAL_H
#define DECAL_H

typedef struct decal decal;

struct decal
{
    SDL_Texture *texture;
    SDL_Rect rect;
};

decal *decal_init(
        decal *d,
        SDL_Texture *texture,
        double x,
        double y,
        double w,
        double h);

void decal_set_xywh(
        decal *d,
        double x,
        double y,
        double w,
        double h);

#endif
