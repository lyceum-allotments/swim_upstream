#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#if EMSCRIPTEN
#include <emscripten.h>
#endif


#include "level1.h"

void setup_textures();
void setup_decals();
void setup_actors();
void setup_active_states();
void loop_handler();

engine *engine_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");

    eng.fps = 80;
    eng.current_frame = 0;

    eng.w = 800;
    eng.h = 600;

    eng.start_time = 0;
    eng.should_start_logic_loop = true;
    eng.whole_frames_to_do = 0;

    eng.swim_start_time = 0;
    eng.swim_time = 0;

    SDL_CreateWindowAndRenderer(eng.w, eng.h, 0, &eng.window, &eng.renderer);

    if(eng.window == NULL) {
        fprintf(
                stderr,
                "Window could not be created: %s\n", SDL_GetError());
        return NULL;
    }

    setup_textures();
    setup_decals();

    eng.render_list = NULL;

    eng.level_bytes = level1_array;

    setup_actors();
    eng.clicked_waypoint = NULL;

    setup_active_states();


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

void setup_textures()
{
    char * filenames[] = {
        "assets/level1.png",
        "assets/waypoint.png",
        "assets/anchorpoint.png",
        "assets/linkline.png",
        "assets/fish.png"
    };
    int i;

    for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
        char * fname = filenames[i];
        SDL_Surface *img = IMG_Load(fname);

        if(!img){
            fprintf(stdout, "Error! Could not load %s\n", fname);
            exit(1);
        }
        
        eng.textures[i] = SDL_CreateTextureFromSurface(eng.renderer, img);
        SDL_FreeSurface(img);
    }
}

void setup_decals()
{
    int imgw, imgh;

    SDL_QueryTexture(eng.textures[BG_TEXTURE],
            NULL, NULL, &imgw, &imgh);

    decal_init(
        &(eng.bg_decal),
        eng.textures[BG_TEXTURE],
        0,
        0,
        imgw,
        imgh);

    decal_init(
        &(eng.waypoint_decal),
        eng.textures[WAYPOINT_TEXTURE],
        0,
        0,
        WAYPOINT_W,
        WAYPOINT_H);

    decal_init(
        &(eng.anchorpoint_decal),
        eng.textures[ANCHORPOINT_TEXTURE],
        0,
        0,
        WAYPOINT_W,
        WAYPOINT_H);

    SDL_QueryTexture(eng.textures[LINKLINE_TEXTURE],
            NULL, NULL, &imgw, &imgh);
    
    decal_init(
        &(eng.linkline_decal),
        eng.textures[LINKLINE_TEXTURE],
        0,
        0,
        imgw,
        imgh);

    decal_init(
        &(eng.fish_decal),
        eng.textures[FISH_TEXTURE],
        0,
        0,
        FISH_W,
        FISH_H);
}

int waypoint_initial_positions[NUM_WAYPOINT_ACTORS][2] = {{200, 0}, {50, 50}, {400, 350}, {600, 500}};
void setup_actors()
{
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.fish_actor));
    eng.logic_list = actor_list_add(eng.logic_list, (actor *)(&eng.fish_actor));

    unsigned int i;

    for (i = 0; i < NUM_WAYPOINT_ACTORS; i++)
    {
        if (i == 0 || i == NUM_WAYPOINT_ACTORS - 1)
            anchorpoint_actor_init(&eng.waypoint_actor[i]);
        else
        {
            waypoint_actor_init(&eng.waypoint_actor[i]);
            eng.logic_list = actor_list_add(eng.logic_list, (actor *)(&eng.waypoint_actor[i]));
        }

        eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.waypoint_actor[i]));
        eng.waypoint_actor[i].sprite.r[0] = waypoint_initial_positions[i][0];
        eng.waypoint_actor[i].sprite.r[1] = waypoint_initial_positions[i][1];
    }

    fish_actor_init(&eng.fish_actor);

    for (i = 0; i < NUM_LINKLINE_ACTORS; i++)
    {
        linkline_actor_init(&eng.linkline_actor[i]);
        eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.linkline_actor[i]));
        

        int wp_x, wp_y;
        waypoint_actor_get_pos(&eng.waypoint_actor[i], &wp_x, &wp_y);
        linkline_actor_move_first_endpoint_to(&eng.linkline_actor[i], wp_x, wp_y);
        eng.waypoint_actor[i].linkline_right = &eng.linkline_actor[i];

        waypoint_actor_get_pos(&eng.waypoint_actor[i+1], &wp_x, &wp_y);
        linkline_actor_move_second_endpoint_to(&eng.linkline_actor[i], wp_x, wp_y);
        eng.waypoint_actor[i+1].linkline_left = &eng.linkline_actor[i];
    }

    bg_actor_init(&eng.bg_actor);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.bg_actor));
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
            || eng.active_states[GAME_STATE_LEVEL_FINISHED])
    {
        return;
    }

    unsigned int i;
    for (i = 0; i< NUM_WAYPOINT_ACTORS; i++)
    {
        if (waypoint_actor_touched(
                    &eng.waypoint_actor[i],
                    eng.x_mouse, eng.y_mouse))
        {
            eng.active_states[GAME_STATE_WAYPOINT_CLICKED] = true;
            eng.clicked_waypoint = &eng.waypoint_actor[i];
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
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                    if (!eng.active_states[GAME_STATE_LEVEL_FINISHED])
                        eng.active_states[GAME_STATE_SWIM_IN_PROGRESS] = true;
                if (event.key.keysym.sym == SDLK_r)
                    eng.active_states[GAME_STATE_LEVEL_RESTART] = true;
                break;
            case SDL_QUIT:
                eng.active_states[GAME_STATE_QUIT] = true;
                break;
        }
    }
}

void eng_logic_handler()
{
    if (eng.active_states[GAME_STATE_LEVEL_RESTART])
    {
        eng.swim_start_time = 0;
        eng.swim_time = 0;

        int i;
        for (i = 0; i < NUM_WAYPOINT_ACTORS; i++)
        {
            eng.waypoint_actor[i].sprite.r[0] = waypoint_initial_positions[i][0];
            eng.waypoint_actor[i].sprite.r[1] = waypoint_initial_positions[i][1];
        }

        for (i = 0; i < NUM_LINKLINE_ACTORS; i++)
        {
            int wp_x, wp_y;
            waypoint_actor_get_pos(&eng.waypoint_actor[i], &wp_x, &wp_y);
            linkline_actor_move_first_endpoint_to(&eng.linkline_actor[i], wp_x, wp_y);

            waypoint_actor_get_pos(&eng.waypoint_actor[i+1], &wp_x, &wp_y);
            linkline_actor_move_second_endpoint_to(&eng.linkline_actor[i], wp_x, wp_y);
        }

        fish_actor_update_next_wp_index(&eng.fish_actor, 1);
        eng.fish_actor.next_wp_index = 0;
        eng.fish_actor.fraction_complete = 1;

        eng.active_states[GAME_STATE_LEVEL_FINISHED] = false;
        eng.active_states[GAME_STATE_LEVEL_RESTART] = false;
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
        eng_logic_handler();
        for (al = eng.logic_list; al != NULL; al = al->next) {
            al->a->logic_handler(al->a);
        }
    }

    SDL_RenderPresent(eng.renderer);
}
