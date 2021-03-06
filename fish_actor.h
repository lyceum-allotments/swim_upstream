#ifndef FISH_ACTOR_H
#define FISH_ACTOR_H
#include "engine.h"

typedef struct fish_actor fish_actor;

struct fish_actor {
    actor a;
    sprite sprite;

    unsigned int player_i;
    double pos[2];
    double speed;

    double direction[2];
    double total_distance_to_travel;
    double fraction_complete;

    unsigned int finish_frames;

    int next_wp_index;
};

fish_actor *fish_actor_init(fish_actor *this, unsigned int player_i);
fish_actor *fish_actor_update_next_wp_index(fish_actor *this, int wp_index);
#endif
