#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#if EMSCRIPTEN
#include <emscripten.h>
#endif

void setup_active_states();
void loop_handler();
void transition_to_p2();

engine *engine_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");
    TTF_Init();

    eng.fps = 80;
    eng.current_frame = 0;

    eng.w = 800;
    eng.h = 600;

    eng.start_time = 0;
    eng.should_start_logic_loop = true;
    eng.whole_frames_to_do = 0;

    eng.target_time_ms = 0;
    eng.frames_swimming = 0;

    SDL_CreateWindowAndRenderer(eng.w, eng.h, 0, &eng.window, &eng.renderer);

    if(eng.window == NULL) {
        fprintf(
                stderr,
                "Window could not be created: %s\n", SDL_GetError());
        return NULL;
    }

    eng.textures[BG_TEXTURE] = NULL;
    setup_active_states();
    eng.play_mode = PLAY_MODE_SP;

    intro_setup();
    eng.scene_finished = intro_finished;
    eng.scene_change = intro_change;
    return &eng;
}

void engine_start()
{
    eng.start_time = SDL_GetTicks();

#ifdef EMSCRIPTEN
    emscripten_set_main_loop(loop_handler, -1, 0);
#else
    while (!eng.active_states[GAME_STATE_QUIT])
    {
        loop_handler();
    }
#endif
}

void setup_active_states()
{
    unsigned int i;
    for (i = 0; i < ARRAY_SIZE(eng.active_states); i++)
    {
        eng.active_states[i] = false;
    }
}

bool should_continue_logic_loops()
{
    if (eng.should_start_logic_loop) {
        unsigned int logic_loop_start_time = SDL_GetTicks();
        double elapsed_frames = (double)(logic_loop_start_time \
                - eng.start_time) / 1000.0f * eng.fps;

        eng.whole_frames_to_do = (unsigned int)elapsed_frames - eng.current_frame;
    }

    if (!eng.whole_frames_to_do) {
        eng.should_start_logic_loop = true;
        return false;
    }

    eng.whole_frames_to_do -= 1;
    eng.current_frame += 1;
    eng.should_start_logic_loop = false;
    return true;
}

void check_waypoint_clicks()
{
    if (eng.active_states[GAME_STATE_SWIM_IN_PROGRESS] 
            || eng.active_states[GAME_STATE_LEVEL_FINISHED]
            || eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO])
    {
        return;
    }

    unsigned int i;
    for (i = 0; i< eng.num_waypoints; i++)
    {
        if (waypoint_actor_touched(
                    &eng.waypoint_actor[eng.active_player_i][i],
                    eng.x_mouse, eng.y_mouse))
        {
            eng.active_states[GAME_STATE_WAYPOINT_CLICKED] = true;
            eng.clicked_waypoint = &eng.waypoint_actor[eng.active_player_i][i];
        }
    }
}

void process_input()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                eng.x_mouse = event.motion.x;
                eng.y_mouse = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                check_waypoint_clicks();
                break;
            case SDL_MOUSEBUTTONUP:
                eng.active_states[GAME_STATE_WAYPOINT_CLICKED] = false;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    if (eng.play_mode == PLAY_MODE_SP)
                    {
                        if (!(eng.active_states[GAME_STATE_LEVEL_FINISHED]
                                    || eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO]))
                            eng.active_states[GAME_STATE_SWIM_IN_PROGRESS] = true;
                    }
                    else if (!(eng.active_states[GAME_STATE_LEVEL_FINISHED]
                                    || eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO]))
                    {
                        if (eng.active_player_i == 0)
                        {
                            eng.active_states[GAME_STATE_TRANSITION_TO_P2] = true;
                        }
                        else // if eng.active_player_i == 1
                        {
                            activate_route(0);
                            eng.render_list = actor_list_add(eng.render_list, (actor *)&eng.fish_actor[0], RENDER_ORDER_FISH);
                            eng.render_list = actor_list_sort(eng.render_list);
                            eng.active_states[GAME_STATE_SWIM_IN_PROGRESS] = true;
                        }
                    }
                }

                if (eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO])
                    if (event.key.keysym.sym == SDLK_RETURN)
                        eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO] = false;

                if (eng.active_states[GAME_STATE_LEVEL_FINISHED])
                {
                    if (event.key.keysym.sym == SDLK_r)
                        eng.active_states[GAME_STATE_LEVEL_RESTART] = true;
                    if (eng.active_states[GAME_STATE_PASSED_CHALLENGE] && event.key.keysym.sym == SDLK_y)
                        eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL] = true;
                }

                if (eng.active_states[GAME_STATE_IN_INTRO])
                {
                    if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        eng.play_mode = multiplayer_select_menu_get_value(&eng.multiplayer_select_menu);
                        eng.active_states[GAME_STATE_INTRO_FINISHED] = true;
                    }
                    else if (event.key.keysym.sym == SDLK_UP)
                    {
                        multiplayer_select_menu_mv_up(&eng.multiplayer_select_menu);
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN)
                    {
                        multiplayer_select_menu_mv_down(&eng.multiplayer_select_menu);
                    }
                }
                break;
            case SDL_QUIT:
                eng.active_states[GAME_STATE_QUIT] = true;
                break;
        }
    }
}

void eng_logic_handler()
{
    if (eng.active_states[GAME_STATE_SWIM_IN_PROGRESS])
    {
        eng.frames_swimming += 1;
    }
}

void loop_handler()
{
    SDL_RenderClear(eng.renderer);
    process_input();

    actor_list *al;
    for (al = eng.render_list; al != NULL; al = al->next) {
        al->a->render_handler(al->a);
    }

    while (should_continue_logic_loops()) {
        if (eng.scene_finished()) {
            eng.scene_change();

        }
        eng_logic_handler();
        for (al = eng.logic_list; al != NULL; al = al->next) {
            al->a->logic_handler(al->a);
        }
    }

    SDL_RenderPresent(eng.renderer);
}

void transition_to_p2()
{
    eng.active_player_i = 1;

    level_intro_screen_refresh_text(&eng.hud_actor.level_intro_screen);
    eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO] = true;
}
