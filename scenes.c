#include "engine.h"

void level1_repeat();
void level1_level2_change();
bool level2_finished();
void level2_repeat();

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
    if (eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL])
    {
        eng.scene_change = level1_level2_change;
        return true;
    }
    if (eng.active_states[GAME_STATE_LEVEL_RESTART])
    {
        eng.scene_change = level1_repeat;
        return true;
    }
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

    eng.scene_change = level1_level2_change;
    return;
}

void level1_repeat()
{
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

    eng.frames_swimming = 0;

    eng.active_states[GAME_STATE_LEVEL_FINISHED] = false;
    eng.active_states[GAME_STATE_LEVEL_RESTART] = false;
}

#include "level2.h"
void level1_level2_change()
{
#include "level2_waypoint_initial_positions.c"

    int i;

    SDL_DestroyTexture(eng.textures[BG_TEXTURE]);
    eng.level_bytes = level2_array;
    static char fname[] = "assets/level2.png";
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

    eng.frames_swimming = 0;

    eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL] = false;
    eng.active_states[GAME_STATE_LEVEL_FINISHED] = false;
    eng.active_states[GAME_STATE_LEVEL_RESTART] = false;

    eng.scene_finished = level2_finished;
    // printf("should transition to level 2 here\n");
    // eng.active_states[GAME_STATE_QUIT] = true;
}

bool level2_finished()
{
    /* if (eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL])
    {
        eng.scene_change = level1_level2_change;
        return true;
    }*/
    if (eng.active_states[GAME_STATE_LEVEL_RESTART])
    {
        eng.scene_change = level2_repeat;
        return true;
    }
    return false;
}

void level2_repeat()
{
#include "level2_waypoint_initial_positions.c"
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

    eng.frames_swimming = 0;

    eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL] = false;
    eng.active_states[GAME_STATE_LEVEL_FINISHED] = false;
    eng.active_states[GAME_STATE_LEVEL_RESTART] = false;

}
