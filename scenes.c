#include "engine.h"

void level1_repeat();
void level1_level2_change();
void level2_level3_change();
bool level2_finished();
void level2_repeat();
void level3_repeat();

void load_bg(const char *img_path)
{
    SDL_Surface *img = IMG_Load(img_path);

    if(!img){
        fprintf(stdout, "Error! Could not load %s\n", img_path);
        exit(1);
    }

    if (eng.textures[BG_TEXTURE])
        SDL_DestroyTexture(eng.textures[BG_TEXTURE]);

    eng.textures[BG_TEXTURE] = SDL_CreateTextureFromSurface(eng.renderer, img);

    int imgw = img->w, imgh = img->h;
    SDL_FreeSurface(img);

    decal_init(
        &(eng.bg_decal),
        eng.textures[BG_TEXTURE],
        0,
        0,
        imgw,
        imgh);
}

void intro_setup()
{
    load_bg("assets/intro_img.png");
    eng.num_waypoints = 0;

    bg_actor_init(&eng.bg_actor);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.bg_actor), RENDER_ORDER_BG);

    multiplayer_select_menu_init(&eng.multiplayer_select_menu);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.multiplayer_select_menu), RENDER_ORDER_MENU);

    eng.render_list = actor_list_sort(eng.render_list);

    eng.active_states[GAME_STATE_IN_INTRO] = true;
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
    if (eng.active_states[GAME_STATE_LEVEL_RESTART] || eng.active_states[GAME_STATE_TRANSITION_TO_P2])
    {
        eng.scene_change = level1_repeat;
        return true;
    }
    return false;
}

#include "level1_setup_helpers.c"
void intro_change()
{
    SDL_DestroyTexture(eng.textures[BG_TEXTURE]);

    eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.multiplayer_select_menu);

    setup_textures();
    setup_decals();

    eng.level_bytes = level1_array;

    hud_actor_init(&eng.hud_actor);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.hud_actor), RENDER_ORDER_HUD);
    eng.logic_list = actor_list_add(eng.render_list, (actor *)(&eng.hud_actor), 0);

    fish_actor_init(&eng.fish_actor[0], 0);
    eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.fish_actor[0]), RENDER_ORDER_FISH);
    eng.logic_list = actor_list_add(eng.logic_list, (actor *)(&eng.fish_actor[0]), 0);

    fish_actor_init(&eng.fish_actor[1], 1);
    if (eng.play_mode == PLAY_MODE_2P)
        eng.logic_list = actor_list_add(eng.logic_list, (actor *)(&eng.fish_actor[1]), 0);

    level1_repeat();

    eng.clicked_waypoint = NULL;
    eng.scene_finished = level1_finished;
    eng.scene_change = level1_level2_change;

    return;
}

void clear_waypoints_off_lists()
{
    int i;
    int j;
    for (i = 0; i < eng.num_waypoints; i++)
    {
        for (j = 0; j < NUM_PLAYERS; j++)
        {
            eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.waypoint_actor[j][i]);
            eng.logic_list = actor_list_rm(eng.logic_list, (actor *)&eng.waypoint_actor[j][i]);

            if (i < eng.num_waypoints - 1)
                eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.linkline_actor[j][i]);
        }
    }
}

void setup_route(waypoint_desc *wpds, int num_wps)
{
    int i;

    eng.num_waypoints = num_wps;

    for (i = 0; i < eng.num_waypoints; i++)
    {
        waypoint_desc *wpd = &wpds[i];
        if (wpd->is_anchorpoint)
            anchorpoint_actor_init(&eng.waypoint_actor[eng.active_player_i][i]);
        else 
        {
            waypoint_actor_init(&eng.waypoint_actor[eng.active_player_i][i]);
            eng.logic_list = actor_list_add(eng.logic_list, (actor *)(&eng.waypoint_actor[eng.active_player_i][i]), 0);
        }

        eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.waypoint_actor[eng.active_player_i][i]), RENDER_ORDER_WAYPOINT);
        eng.waypoint_actor[eng.active_player_i][i].sprite.r[0] = wpd->x;
        eng.waypoint_actor[eng.active_player_i][i].sprite.r[1] = wpd->y;
    }

    for (i = 0; i < eng.num_waypoints - 1; i++)
    {
        linkline_actor_init(&eng.linkline_actor[eng.active_player_i][i]);
        eng.render_list = actor_list_add(eng.render_list, (actor *)(&eng.linkline_actor[eng.active_player_i][i]), RENDER_ORDER_LINKLINE);

        int wp_x, wp_y;
        waypoint_actor_get_pos(&eng.waypoint_actor[eng.active_player_i][i], &wp_x, &wp_y);
        linkline_actor_move_first_endpoint_to(&eng.linkline_actor[eng.active_player_i][i], wp_x, wp_y);
        eng.waypoint_actor[eng.active_player_i][i].linkline_right = &eng.linkline_actor[eng.active_player_i][i];

        waypoint_actor_get_pos(&eng.waypoint_actor[eng.active_player_i][i+1], &wp_x, &wp_y);
        linkline_actor_move_second_endpoint_to(&eng.linkline_actor[eng.active_player_i][i], wp_x, wp_y);
        eng.waypoint_actor[eng.active_player_i][i+1].linkline_left = &eng.linkline_actor[eng.active_player_i][i];
    }
    eng.render_list = actor_list_sort(eng.render_list);
    eng.logic_list = actor_list_sort(eng.logic_list);

    fish_actor_init(&eng.fish_actor[eng.active_player_i], eng.active_player_i);
}

void inactivate_route(unsigned int player_i)
{
    int i;

    for (i = 0; i < eng.num_waypoints; i++)
    {
        eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.waypoint_actor[player_i][i]);
        eng.render_list = actor_list_add(eng.render_list, (actor *)&eng.waypoint_actor[player_i][i], RENDER_ORDER_INACTIVE_WAYPOINT);
        waypoint_actor_make_inactive(&eng.waypoint_actor[player_i][i]);

        if (i < eng.num_waypoints - 1)
        {
            eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.linkline_actor[player_i][i]);
            eng.render_list = actor_list_add(eng.render_list, (actor *)&eng.linkline_actor[player_i][i], RENDER_ORDER_INACTIVE_LINKLINE);
            linkline_actor_make_inactive(&eng.linkline_actor[player_i][i]);
        }
    }
}

void activate_route(unsigned int player_i)
{
    int i;

    for (i = 0; i < eng.num_waypoints; i++)
    {
        waypoint_actor_make_active(&eng.waypoint_actor[player_i][i]);

        if (i < eng.num_waypoints - 1)
        {
            linkline_actor_make_active(&eng.linkline_actor[player_i][i]);
        }
    }
}

void level_light_setup(waypoint_desc *waypoints, unsigned int waypoints_size, unsigned int target_time_ms)
{
    if (eng.active_states[GAME_STATE_TRANSITION_TO_P2])
    {
        inactivate_route(eng.active_player_i);
        eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.fish_actor[0]);
        eng.render_list = actor_list_add(eng.render_list, (actor *)&eng.fish_actor[1], RENDER_ORDER_FISH);
        eng.active_player_i = 1;
    }
    else
    {
        eng.render_list = actor_list_rm(eng.render_list, (actor *)&eng.fish_actor[1]);
        eng.active_player_i = 0;
        clear_waypoints_off_lists();
    }

    setup_route(waypoints, waypoints_size);
    eng.active_states[GAME_STATE_SHOW_LEVEL_INTRO] = true;

    eng.target_time_ms= target_time_ms;
    eng.frames_swimming = 0;
    level_intro_screen_refresh_text(&eng.hud_actor.level_intro_screen);

    eng.active_states[GAME_STATE_TRANSITION_TO_P2] = false;
    eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL] = false;
    eng.active_states[GAME_STATE_PASSED_CHALLENGE] = false;
    eng.active_states[GAME_STATE_LEVEL_FINISHED] = false;
    eng.active_states[GAME_STATE_LEVEL_RESTART] = false;

}

void level1_repeat()
{
#include "level1_waypoint_initial_positions.h"
    level_light_setup(waypoint_desc, sizeof(waypoint_desc)/sizeof(waypoint_desc[0]), target_time_ms);
}

#include "level2.h"
void level1_level2_change()
{
    eng.level_bytes = level2_array;
    load_bg("assets/level2.png");

    level2_repeat();

    eng.scene_finished = level2_finished;
}

bool level2_finished()
{
    if (eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL])
    {
        eng.scene_change = level2_level3_change;
        return true;
    }
    if (eng.active_states[GAME_STATE_LEVEL_RESTART] || eng.active_states[GAME_STATE_TRANSITION_TO_P2])
    {
        eng.scene_change = level2_repeat;
        return true;
    }
    return false;
}

void level2_repeat()
{
#include "level2_waypoint_initial_positions.c"
    level_light_setup(waypoint_desc, sizeof(waypoint_desc)/sizeof(waypoint_desc[0]), target_time_ms);
}

bool level3_finished()
{
    // if (eng.active_states[GAME_STATE_PROGRESS_TO_NEXT_LEVEL])
    // {
    //     eng.scene_change = level2_level3_change;
    //     return true;
    // }
    if (eng.active_states[GAME_STATE_LEVEL_RESTART] || eng.active_states[GAME_STATE_TRANSITION_TO_P2])
    {
        eng.scene_change = level3_repeat;
        return true;
    }
    return false;
}

#include "level3.h"
void level2_level3_change()
{
    load_bg("assets/level3.png");

    level3_repeat();
    eng.scene_finished = level3_finished;
}

void level3_repeat()
{
#include "level3_waypoint_initial_positions.c"
    level_light_setup(waypoint_desc, sizeof(waypoint_desc)/sizeof(waypoint_desc[0]), target_time_ms);
}

