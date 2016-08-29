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
    waypoint_actor_get_pos(&eng.waypoint_actor[this->wp_from_index], &x_from, &y_from);
    waypoint_actor_get_pos(&eng.waypoint_actor[this->wp_to_index], &x_to, &y_to);

    this->direction[0] = (double)(x_to - x_from);
    this->direction[1] = (double)(y_to - y_from);

    this->total_distance_to_travel = vec_mag(this->direction[0], this->direction[1]);

    vec_norm(&this->direction[0], &this->direction[1]);
}

void fish_logic_handler(actor *a)
{
   fish_actor *fish = (fish_actor *)a;

   if (fish->wp_to_index == NUM_WAYPOINT_ACTORS)
       return;

   fish->pos[0] += fish->speed * fish->direction[0];
   fish->pos[1] += fish->speed * fish->direction[1];

   fish->fraction_complete += fish->speed / fish->total_distance_to_travel;

   if (fish->fraction_complete > 1)
   {
       fish->wp_from_index += 1;
       fish->wp_to_index += 1;

       int x, y;
       waypoint_actor_get_pos(&eng.waypoint_actor[fish->wp_from_index], &x, &y);
       fish->pos[0] = (double)x;
       fish->pos[1] = (double)y;

       calculate_direction_and_distance(fish);
       fish->fraction_complete = 0;
   }
}

fish_actor *fish_actor_init(fish_actor *this, int wp_from_index, int wp_to_index)
{
    actor_init(&this->a, fish_render_handler, fish_logic_handler);
    sprite_init(
            &this->sprite,
            FISH_W,
            FISH_H,
            &eng.fish_decal);
    this->sprite.r[0] = 0;
    this->sprite.r[1] = 0;

    this->wp_from_index = wp_from_index;
    this->wp_to_index = wp_to_index;

    int x, y;
    waypoint_actor_get_pos(&eng.waypoint_actor[wp_from_index], &x, &y);
    this->pos[0] = (double)x;
    this->pos[1] = (double)y;

    this->speed = 1.0;

    calculate_direction_and_distance(this);

    return this;
}
