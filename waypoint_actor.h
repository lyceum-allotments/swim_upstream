#ifndef WAYPOINT_ACTOR_H
#define WAYPOINT_ACTOR_H
#include "engine.h"

typedef struct waypoint_actor waypoint_actor;

struct waypoint_actor {
    actor a;
    sprite sprite;

    linkline_actor *linkline_left;
    linkline_actor *linkline_right;
};

waypoint_actor *waypoint_actor_init(waypoint_actor *wpa);
void waypoint_actor_get_pos(waypoint_actor * this, int * x, int * y);
void waypoint_actor_set_pos(waypoint_actor * this, int x, int y);
bool waypoint_actor_touched(waypoint_actor * wpa, unsigned int x, unsigned int y);
waypoint_actor *waypoint_actor_make_inactive(waypoint_actor *this);
waypoint_actor *waypoint_actor_make_active(waypoint_actor *this);

waypoint_actor *anchorpoint_actor_init(waypoint_actor *this);
#endif
