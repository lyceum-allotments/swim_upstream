#ifndef ACTOR_H
#define ACTOR_H

typedef struct actor actor;
typedef void (*actor_render_handler)(actor *a);
typedef void (*actor_logic_handler)(actor *a);

struct actor
{
    actor_render_handler render_handler;
    actor_logic_handler logic_handler;
};

typedef struct actor_list actor_list;
struct actor_list
{
    actor *a;
    actor_list *next;
    unsigned int priority;
};


actor_list *actor_list_add(actor_list *al, actor *a, unsigned int priority);
actor_list *actor_list_rm(actor_list *al, actor *a);
actor_list *actor_list_sort(actor_list *al);

actor *actor_init(
        actor *a,
        actor_render_handler render_handler,
        actor_logic_handler logic_handler);

#endif
