#include "engine.h"

void linkline_render_handler(actor *a)
{
    linkline_actor *la = (linkline_actor *)a;
    sprite_render(&(la->sprite));
}

linkline_actor *linkline_actor_init(linkline_actor *lla)
{
    actor_init(&lla->a, linkline_render_handler, NULL);
    sprite_init(
            &lla->sprite,
            10 * WAYPOINT_W,
            LINKLINE_W,
            &eng.linkline_decal);
    lla->sprite.r[0] = 0;
    lla->sprite.r[1] = 0;

    return lla;
}

void linkline_actor_refresh_sprite(linkline_actor *this)
{
    this->sprite.r[0] = this->endpoint[0][0];
    this->sprite.r[1] = this->endpoint[0][1] - LINKLINE_W / 2;

    this->sprite.rot_centre[0] = 0;
    this->sprite.rot_centre[1] = LINKLINE_W / 2;

    int vec[2] = {this->endpoint[1][0] - this->endpoint[0][0], this->endpoint[1][1] - this->endpoint[0][1]};
    this->sprite.w = (int)vec_mag(vec[0], vec[1]);

    this->sprite.rot_angle = vec_arg(vec[0], vec[1]);
}

void linkline_actor_move_first_endpoint_to(linkline_actor *this, int x, int y)
{
    this->endpoint[0][0] = x;
    this->endpoint[0][1] = y;

    linkline_actor_refresh_sprite(this);
}
void linkline_actor_move_second_endpoint_to(linkline_actor *this, int x, int y)
{
    this->endpoint[1][0] = x;
    this->endpoint[1][1] = y;
    linkline_actor_refresh_sprite(this);
}

linkline_actor *linkline_actor_make_inactive(linkline_actor *this)
{
    sprite_set_decal(&this->sprite, &eng.linkline_inactive_decal);
    return this;
}

linkline_actor *linkline_actor_make_active(linkline_actor *this)
{
    sprite_set_decal(&this->sprite, &eng.linkline_decal);
    return this;
}
