#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

//add a delete stack function that frees everything

/*
 * Basic stack functions
 */
void push(stack *s, element *e)
{
    if(!is_empty(s))
    {
        s->top->next = e;
        e->prev = s->top;
        s->top = e;
    }
    else
        s->top = e;
    return;
}

element *pop(stack *s)
{
    if(!is_empty(s))
    {
        element *popped = s->top;
        s->top = s->top->prev;
        popped->prev = NULL;
        return popped;
    }
    else
        return NULL;
}

element *peek(stack *s)
{
    if(!is_empty(s))
        return s->top;
    else
        return NULL;
}

int is_empty(stack *s)
{
    return s->top == NULL;
}

element *init_element(char *data)
{
    element *e = malloc(sizeof(element));
    e->data = strdup(data);
    return e;
}

stack *init_stack()
{
    stack *s = malloc(sizeof(stack));
    return s;
}
