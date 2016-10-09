#include "engine.h"

void intro_setup()
{
    char * fname = "assets/intro_img.png";
    SDL_Surface *img = IMG_Load(fname);

    if(!img){
        fprintf(stdout, "Error! Could not load %s\n", fname);
        exit(1);
    }

    eng.textures[BG_TEXTURE] = SDL_CreateTextureFromSurface(eng.renderer, img);
    SDL_FreeSurface(img);

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

    bg_actor_init(&eng.bg_actor);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.bg_actor));
    return;
}

bool intro_finished() 
{
    if (eng.active_states[GAME_STATE_INTRO_FINISHED])
        return true;
    return false;
}

bool level1_finished()
{
    return false;
}

#include "level1_setup_helpers.c"
void intro_change()
{
    eng.render_list = actor_list_rm(eng.render_list, (actor *)(&eng.bg_actor));
    SDL_DestroyTexture(eng.textures[BG_TEXTURE]);

    setup_textures();
    setup_decals();

    eng.level_bytes = level1_array;

    setup_actors();
    eng.clicked_waypoint = NULL;
    eng.scene_finished = level1_finished;
    return;
}
