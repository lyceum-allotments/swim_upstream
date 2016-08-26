#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "utils.h"
#include "sprite.h"
#include "actor.h"
#include "bg_actor.h"
#include "waypoint_actor.h"

enum
{
    BG_TEXTURE,
    WAYPOINT_TEXTURE,
    NUM_TEXTURES
};

typedef enum GAME_STATE
{
    GAME_STATE_WAYPOINT_CLICKED,
    GAME_STATE_QUIT,
    GAME_STATE_NUM_STATES
} GAME_STATE;

typedef struct engine engine;
struct engine
{
    unsigned int fps;
    unsigned int current_frame;

    unsigned int w;
    unsigned int h;

    Uint32 start_time;
    bool should_start_logic_loop;
    unsigned int whole_frames_to_do;

    SDL_Window *window;
    SDL_Renderer *renderer;

    // mouse coords this frame
    Sint32 x_mouse;
    Sint32 y_mouse;

    bool active_states[GAME_STATE_NUM_STATES];

    actor_list *render_list;
    actor_list *logic_list;

    SDL_Texture *textures[NUM_TEXTURES];
    decal bg_decal;
    decal waypoint_decal;

    bg_actor bg_actor;

    waypoint_actor waypoint_actor;
};

enum
{
    WAYPOINT_W = 30,
    WAYPOINT_H = 30
};

engine eng;

engine *engine_init();

void engine_start();

#endif
