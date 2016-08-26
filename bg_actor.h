#ifndef BG_ACTOR_H
#define BG_ACTOR_H
#include "engine.h"

typedef struct bg_actor bg_actor;

struct bg_actor {
    actor a;
    sprite sprite;
};

bg_actor *bg_actor_init(bg_actor *bg);
#endif
