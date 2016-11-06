#include "engine.h"

void waypoint_render_handler(actor *a)
{
    waypoint_actor *wpa = (waypoint_actor *)a;
    sprite_render(&(wpa->sprite));
}

void waypoint_logic_handler(actor *a)
{
   waypoint_actor *wpa = (waypoint_actor *)a;

   if (eng.active_states[GAME_STATE_WAYPOINT_CLICKED] && eng.clicked_waypoint == wpa)
   {
       waypoint_actor_set_pos(wpa, eng.x_mouse, eng.y_mouse);
   }
}

waypoint_actor *waypoint_actor_init(waypoint_actor *wpa)
{
    actor_init(&wpa->a, waypoint_render_handler, waypoint_logic_handler);
    sprite_init(
            &wpa->sprite,
            WAYPOINT_W,
            WAYPOINT_H,
            &eng.waypoint_decal);
    wpa->sprite.r[0] = 0;
    wpa->sprite.r[1] = 0;

    wpa->linkline_left = NULL;
    wpa->linkline_right = NULL;

    return wpa;
}

void waypoint_actor_get_pos(waypoint_actor * this, int * x, int * y)
{
    *x = this->sprite.r[0] + WAYPOINT_W / 2;
    *y = this->sprite.r[1] + WAYPOINT_H / 2;
}

void waypoint_actor_set_pos(waypoint_actor * this, int x, int y)
{
    this->sprite.r[0] = x - WAYPOINT_W / 2;
    this->sprite.r[1] = y - WAYPOINT_H / 2;

    if (this->linkline_left)
        linkline_actor_move_second_endpoint_to(this->linkline_left, x, y);
    if (this->linkline_right)
        linkline_actor_move_first_endpoint_to(this->linkline_right, x, y);
}

bool waypoint_actor_touched(waypoint_actor * wpa, unsigned int x, unsigned int y)
{
    return vec_mag_sq(x - (wpa->sprite.r[0] + WAYPOINT_W / 2.), y - (wpa->sprite.r[1] + WAYPOINT_W / 2.)) < (WAYPOINT_W / 2.) * (WAYPOINT_W / 2.);
}


waypoint_actor *anchorpoint_actor_init(waypoint_actor *this)
{
    actor_init(&this->a, waypoint_render_handler, NULL);
    sprite_init(
            &this->sprite,
            WAYPOINT_W,
            WAYPOINT_H,
            &eng.anchorpoint_decal);
    this->sprite.r[0] = 0;
    this->sprite.r[1] = 0;

    this->linkline_left = NULL;
    this->linkline_right = NULL;

    return this;
}

waypoint_actor *waypoint_actor_make_inactive(waypoint_actor *this)
{
    printf("should be changing the waypoint sprite to look inactive here\n");
    return this;
}
