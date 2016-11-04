#ifndef LEVEL_INTRO_SCREEN_H
#define LEVEL_INTRO_SCREEN_H
#include "engine.h"

typedef struct level_intro_screen level_intro_screen;

struct level_intro_screen {
    TTF_Font *heading_font;
    TTF_Font *body_font;

    SDL_Texture *heading_text;
    SDL_Rect heading_rect;

    SDL_Texture *body_text;
    SDL_Rect body_rect;
};

level_intro_screen *level_intro_screen_init(level_intro_screen *this);
level_intro_screen *level_intro_screen_refresh_text(level_intro_screen *this);
level_intro_screen *level_intro_screen_render(level_intro_screen *this);
#endif
