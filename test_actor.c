#include "actor.h"
#include <stdio.h>

int main()
{
    actor_list *test_list = NULL;
    actor a[7];
    unsigned int inp[7] = {3,1,4,1,5,9,2};
    unsigned int exp[7] = {1,1,2,3,4,5,9};
    int i;

    for (i = 0; i < sizeof(a)/sizeof(a[0]); i++)
    {
        test_list = actor_list_add(test_list, &a[i], inp[i]);
    }

    test_list = actor_list_sort(test_list);

    actor_list *p;
    i = 0;
    char passed = 1;
    for (p = test_list; p!=NULL; p=p->next)
    {
        passed = passed && (p->priority == exp[i]);    
        i++;
    }

    if (!passed)
        printf("TEST WAS FAILED\n");
}
