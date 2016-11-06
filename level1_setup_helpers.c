#include "level1.h"

void setup_textures()
{
    const char * filenames[] = {
        "assets/level1.png",
        "assets/waypoint.png",
        "assets/waypoint_inactive.png",
        "assets/anchorpoint.png",
        "assets/linkline.png",
        "assets/linkline_inactive.png",
        "assets/fish.png",
        "assets/fish2.png",
    };
    int i;

    for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
        const char * fname = filenames[i];
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
        &(eng.waypoint_inactive_decal),
        eng.textures[WAYPOINT_INACTIVE_TEXTURE],
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
        &(eng.linkline_inactive_decal),
        eng.textures[LINKLINE_INACTIVE_TEXTURE],
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

    decal_init(
        &(eng.fish_2_decal),
        eng.textures[FISH_2_TEXTURE],
        0,
        0,
        FISH_W,
        FISH_H);
}
