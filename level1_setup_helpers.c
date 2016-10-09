#include "level1_waypoint_initial_positions.h"
#include "level1.h"

void setup_actors()
{
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.hud_actor));
    eng.logic_list = actor_list_add(eng.render_list, (actor *)(&eng.hud_actor));
    hud_actor_init(&eng.hud_actor);

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
