#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>

void setup_textures();
void setup_decals();
void setup_actors();
void loop_handler();

engine *engine_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    eng.fps = 80;
    eng.current_frame = 0;

    eng.w = 800;
    eng.h = 600;

    eng.start_time = 0;
    eng.should_start_logic_loop = true;
    eng.whole_frames_to_do = 0;

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

    setup_actors();

    return &eng;
}

void engine_start()
{
    eng.start_time = SDL_GetTicks();

    emscripten_set_main_loop(loop_handler, -1, 0);
}

void setup_textures()
{
    char * filenames[] = {
        "assets/bg.png",
        "assets/waypoint.png"
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
}

void setup_actors()
{
    waypoint_actor_init(&eng.waypoint_actor);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.waypoint_actor));
    eng.logic_list = actor_list_add(eng.logic_list, (actor *)(&eng.waypoint_actor));

    bg_actor_init(&eng.bg_actor);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.bg_actor));
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

void loop_handler()
{
    SDL_RenderClear(eng.renderer);

    actor_list *al;
    for (al = eng.render_list; al != NULL; al = al->next) {
        al->a->render_handler(al->a);
    }

    while (should_continue_logic_loops()) {
        for (al = eng.logic_list; al != NULL; al = al->next) {
            al->a->logic_handler(al->a);
        }
    }

    SDL_RenderPresent(eng.renderer);
}
