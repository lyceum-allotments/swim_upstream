#ifndef LEVEL_END_SCREEN_H
#define LEVEL_END_SCREEN_H
#include "engine.h"

typedef struct level_end_screen level_end_screen;

struct level_end_screen {
    TTF_Font *intro_font;
    SDL_Texture *intro_text;
    SDL_Rect intro_rect;

    TTF_Font *time_font;
    SDL_Texture *time_text;
    SDL_Rect time_rect;
};

level_end_screen *level_end_screen_init(level_end_screen *this);
level_end_screen *level_end_screen_render(level_end_screen *this);
level_end_screen *level_end_screen_time_refresh(level_end_screen *this);
#endif
