#ifndef FISH_ACTOR_H
#define FISH_ACTOR_H
#include "engine.h"

typedef struct fish_actor fish_actor;

struct fish_actor {
    actor a;
    sprite sprite;

    double pos[2];
    double speed;

    double direction[2];
    double total_distance_to_travel;
    double fraction_complete;

    int next_wp_index;
};

fish_actor *fish_actor_init(fish_actor *this);
fish_actor *fish_actor_update_next_wp_index(fish_actor *this, int wp_index);
#endif
