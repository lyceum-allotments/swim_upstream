#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "utils.h"
#include "sprite.h"
#include "actor.h"
#include "bg_actor.h"
#include "linkline_actor.h"
#include "waypoint_actor.h"
#include "fish_actor.h"

enum
{
    BG_TEXTURE,
    WAYPOINT_TEXTURE,
    ANCHORPOINT_TEXTURE,
    LINKLINE_TEXTURE,
    FISH_TEXTURE,
    NUM_TEXTURES
};

typedef enum GAME_STATE
{
    GAME_STATE_WAYPOINT_CLICKED,
    GAME_STATE_SWIM_IN_PROGRESS,
    GAME_STATE_QUIT,
    GAME_STATE_NUM_STATES
} GAME_STATE;

enum
{
    WAYPOINT_W = 30,
    WAYPOINT_H = 30,
    LINKLINE_W = 18,
    FISH_W = 50,
    FISH_H = 50,
    NUM_WAYPOINT_ACTORS = 4,
    NUM_LINKLINE_ACTORS = NUM_WAYPOINT_ACTORS - 1
};

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
    decal anchorpoint_decal;
    decal linkline_decal;
    decal fish_decal;

    bg_actor bg_actor;

    waypoint_actor waypoint_actor[NUM_WAYPOINT_ACTORS];
    waypoint_actor *clicked_waypoint;

    linkline_actor linkline_actor[NUM_LINKLINE_ACTORS];

    fish_actor fish_actor;

    unsigned char *level_bytes;
};

engine eng;

engine *engine_init();

void engine_start();

#endif
