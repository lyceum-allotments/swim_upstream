#include "engine.h"

void fish_render_handler(actor *a)
{
    fish_actor *fish = (fish_actor *)a;

    fish->sprite.r[0] = (int)fish->pos[0] - FISH_W/2;
    fish->sprite.r[1] = (int)fish->pos[1] - FISH_H/2;

    sprite_render(&(fish->sprite));
}

void calculate_direction_and_distance(fish_actor *this)
{
    int x_from, y_from, x_to, y_to;
    waypoint_actor_get_pos(&eng.waypoint_actor[this->next_wp_index-1], &x_from, &y_from);
    waypoint_actor_get_pos(&eng.waypoint_actor[this->next_wp_index], &x_to, &y_to);

    this->direction[0] = (double)(x_to - x_from);
    this->direction[1] = (double)(y_to - y_from);

    this->total_distance_to_travel = vec_mag(this->direction[0], this->direction[1]);

    vec_norm(&this->direction[0], &this->direction[1]);
}

void fish_logic_handler(actor *a)
{
    if (!eng.active_states[GAME_STATE_SWIM_IN_PROGRESS]
            || eng.active_states[GAME_STATE_LEVEL_FINISHED])
        return;

    fish_actor *fish = (fish_actor *)a;

    if (fish->next_wp_index == eng.num_waypoints)
    {
        eng.active_states[GAME_STATE_SWIM_IN_PROGRESS] = false;
        eng.active_states[GAME_STATE_LEVEL_FINISHED] = true;
        level_end_screen_time_refresh(&eng.hud_actor.level_end_screen);
        return;
    }

    unsigned char level_byte = eng.level_bytes[80 * (int)(fish->pos[1]/10) + (int)(fish->pos[0]/10)];

    double speed_multiplier = (9 * (double)level_byte / 0xFF) + 1;

    fish->pos[0] += fish->speed * fish->direction[0] * speed_multiplier;
    fish->pos[1] += fish->speed * fish->direction[1] * speed_multiplier;

    fish->fraction_complete += (fish->speed * speed_multiplier) / fish->total_distance_to_travel;

    if (fish->fraction_complete > 1)
    {
        fish_actor_update_next_wp_index(fish, fish->next_wp_index + 1);
    }
}

fish_actor *fish_actor_update_next_wp_index(fish_actor * this, int wp_index)
{
    this->next_wp_index = wp_index;

    int x, y;
    waypoint_actor_get_pos(&eng.waypoint_actor[this->next_wp_index-1], &x, &y);
    this->pos[0] = (double)x;
    this->pos[1] = (double)y;

    calculate_direction_and_distance(this);
    this->fraction_complete = 0;
    return this;
}

fish_actor *fish_actor_init(fish_actor *this)
{
    actor_init(&this->a, fish_render_handler, fish_logic_handler);
    sprite_init(
            &this->sprite,
            FISH_W,
            FISH_H,
            &eng.fish_decal);
    this->sprite.r[0] = 0;
    this->sprite.r[1] = 0;

    this->next_wp_index = 0;

    int x, y;
    waypoint_actor_get_pos(&eng.waypoint_actor[this->next_wp_index], &x, &y);
    this->pos[0] = (double)x;
    this->pos[1] = (double)y;
    this->direction[0] = 0;
    this->direction[1] = 0;

    this->speed = 0.25;

    this->fraction_complete = 1;

    return this;
}
