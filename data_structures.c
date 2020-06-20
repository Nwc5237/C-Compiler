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

//utility function for converting ints to ascii strings
char *itoa(int a)
{
    if(a < 0)
    {
        printf("Error: converting negative constant.\n");
        exit(0);
    }

    int log10=0, copy=a;
    char *str;

    if(!a)
        return strdup("0");

    while(copy > 0)
    {
        copy /= 10;
        log10++;
    }

    str = malloc(sizeof(char) * log10);
    str[log10] = '\0';
    log10--;

    while(log10 >= 0)
    {
        str[log10] = (char) (a%10+48);
        a /= 10;
        log10--;
    }
    return str;
}
