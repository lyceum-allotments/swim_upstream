#ifndef LINKLINE_ACTOR_H
#define LINKLINE_ACTOR_H
#include "engine.h"

typedef struct linkline_actor linkline_actor;

struct linkline_actor {
    actor a;
    sprite sprite;

    int endpoint[2][2];
};

linkline_actor *linkline_actor_init(linkline_actor *wpa);
void linkline_actor_move_first_endpoint_to(linkline_actor *this, int x, int y);
void linkline_actor_move_second_endpoint_to(linkline_actor *this, int x, int y);
linkline_actor *linkline_actor_make_inactive(linkline_actor *this);
#endif
