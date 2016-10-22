#include "actor.h"
#include <stdlib.h>
#include <stdio.h>

actor_list *actor_list_add(actor_list *al, actor *a, unsigned int priority)
{
    actor_list *new_node = malloc(sizeof(*new_node));
    new_node->a = a;
    new_node->next = al;
    new_node->priority = priority;
    return new_node;
}

actor_list *actor_list_rm(actor_list *al, actor *a)
{
    if (al == NULL)
        return NULL;
    if (al->a == a)
    {
        actor_list *next = al->next;
        free(al);
        return next;
    }
    al->next = actor_list_rm(al->next, a);
    return al;
}

actor_list *actor_list_sort(actor_list *al)
{
    if (al == NULL)
        return al;

    actor_list *less_than = NULL;
    actor_list *more_than = NULL;

    actor_list *ai;
    for (ai = al->next; ai != NULL;)
    {
        actor_list *next = ai->next;

        if (ai->priority > al->priority)
        {
            ai->next = more_than;
            more_than = ai;
        }
        else
        {
            ai->next = less_than;
            less_than = ai;
        }

        ai = next;
    }

    al->next = actor_list_sort(more_than);

    if (less_than == NULL)
        return al;

    less_than = actor_list_sort(less_than);

    for (ai = less_than; ai != NULL; ai = ai->next)
    {
        if (ai->next == NULL)
        {
            ai->next = al;
            break;
        }
    }
    return less_than;
}

actor *actor_init(
        actor *a,
        actor_render_handler render_handler,
        actor_logic_handler logic_handler)
{
    a->render_handler = render_handler;
    a->logic_handler = logic_handler;
    return a;
}
