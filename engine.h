#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>

#include "utils.h"
#include "sprite.h"
#include "actor.h"
#include "hud_actor.h"
#include "multiplayer_select_menu.h"
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
    GAME_STATE_LEVEL_FINISHED,
    GAME_STATE_PASSED_CHALLENGE,
    GAME_STATE_LEVEL_RESTART,
    GAME_STATE_QUIT,
    GAME_STATE_IN_INTRO,
    GAME_STATE_INTRO_FINISHED,
    GAME_STATE_PROGRESS_TO_NEXT_LEVEL,
    GAME_STATE_NUM_STATES,
} GAME_STATE;

enum
{
    WAYPOINT_W = 30,
    WAYPOINT_H = 30,
    LINKLINE_W = 18,
    FISH_W = 50,
    FISH_H = 50,
    MAX_NUM_WAYPOINT_ACTORS = 10,
    TIMER_TEXT_SIZE = 30,
    END_TIME_TEXT_SIZE = 80
};

typedef enum RENDER_ORDER // things earlier in this list will be rendered first
                          // (i.e. be further back)
{
    RENDER_ORDER_BG,
    RENDER_ORDER_MENU,
    RENDER_ORDER_LINKLINE,
    RENDER_ORDER_WAYPOINT,
    RENDER_ORDER_FISH,
    RENDER_ORDER_HUD
} RENDER_ORDER;

typedef struct waypoint_desc
{
    unsigned int x;
    unsigned int y;
    bool is_anchorpoint;
} waypoint_desc;

#define TIMER_TEXT_FONT "assets/FreeSans.ttf"

typedef struct engine engine;
typedef bool (*scene_finished_check)();
typedef void (*scene_changed_handler)();
#include "scenes.h"

struct engine
{
    unsigned int fps;
    unsigned int current_frame;

    unsigned int w;
    unsigned int h;

    Uint32 start_time;
    bool should_start_logic_loop;
    unsigned int whole_frames_to_do;

    unsigned int frames_swimming;

    unsigned int target_time_ms;

    SDL_Window *window;
    SDL_Renderer *renderer;

    // mouse coords this frame
    Sint32 x_mouse;
    Sint32 y_mouse;

    scene_finished_check scene_finished;
    scene_changed_handler scene_change;

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

    waypoint_actor waypoint_actor[MAX_NUM_WAYPOINT_ACTORS];
    unsigned int num_waypoints;
    waypoint_actor *clicked_waypoint;

    linkline_actor linkline_actor[MAX_NUM_WAYPOINT_ACTORS - 1];

    fish_actor fish_actor;

    hud_actor hud_actor;
    multiplayer_select_menu multiplayer_select_menu;

    const unsigned char *level_bytes;
};

engine eng;

engine *engine_init();

void engine_start();

#endif
