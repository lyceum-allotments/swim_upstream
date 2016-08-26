#ifndef WAYPOINT_ACTOR_H
#define WAYPOINT_ACTOR_H
#include "engine.h"

typedef struct waypoint_actor waypoint_actor;

struct waypoint_actor {
    actor a;
    sprite sprite;
};

waypoint_actor *waypoint_actor_init(waypoint_actor *wpa);
#endif
