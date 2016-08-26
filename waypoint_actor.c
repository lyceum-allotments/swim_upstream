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
       wpa->sprite.r[0] = eng.x_mouse - WAYPOINT_W / 2;
       wpa->sprite.r[1] = eng.y_mouse - WAYPOINT_H / 2;
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

    return wpa;
}

bool waypoint_actor_touched(waypoint_actor * wpa, unsigned int x, unsigned int y)
{
    return vec_mag_sq(x - (wpa->sprite.r[0] + WAYPOINT_W / 2.), y - (wpa->sprite.r[1] + WAYPOINT_W / 2.)) < (WAYPOINT_W / 2.) * (WAYPOINT_W / 2.);
}
