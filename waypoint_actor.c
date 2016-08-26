#include "engine.h"

void waypoint_render_handler(actor *a)
{
    waypoint_actor *wpa = (waypoint_actor *)a;
    sprite_render(&(wpa->sprite));
}

void waypoint_logic_handler(actor *a)
{
   waypoint_actor *wap = (waypoint_actor *)a;
   wap->sprite.r[0] += 1;
}

waypoint_actor *waypoint_actor_init(waypoint_actor *wpa)
{
    actor_init(&wpa->a, waypoint_render_handler, waypoint_logic_handler);
    sprite_init(
            &wpa->sprite,
            WAYPOINT_W,
            WAYPOINT_H,
            &eng.waypoint_decal);
    wpa->sprite.r[0] = 40;
    wpa->sprite.r[1] = 40;

    return wpa;
}
