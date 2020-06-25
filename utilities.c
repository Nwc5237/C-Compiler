#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

/*
 * initiales and returns an empty symbol table
 */
symbol_table_t *init_symbol_table(int size)
{
    symbol_table_t *symbol_table = malloc(sizeof(symbol_table_t));
    symbol_table->symbols = calloc(size, sizeof(symbol_t));
    symbol_table->size = size;
    return symbol_table;
}

/*
 * inserts a new symbol into the symbol table
 */
void insert_symbol_table(symbol_table_t *symbol_table, char *id, type_union_t val)
{
    int table_index, char_sum=0, i;
    symbol_t *sym, *new_sym = calloc(1, sizeof(symbol_t));
    
    for(i=0; i < strlen(id); i++)
        char_sum += (int) id[i];

    table_index = char_sum % symbol_table->size;
    sym = &symbol_table->symbols[table_index];

    if(!sym)
    {
        new_sym->next = NULL;
        new_sym->prev = NULL;
        new_sym->id = id;
        new_sym->val = val;
        new_sym->initialized = 1;
        symbol_table->symbols[table_index] = *new_sym;
        return;
    }

    while(sym && sym->next)
        sym = sym->next;

    new_sym->next = NULL;
    new_sym->prev = sym;
    new_sym->id = id;
    new_sym->val = val;
    new_sym->initialized = 1;
    symbol_table->symbols[table_index] = *new_sym;
    return;
}

/*
 * returns the symbol requested of the symbol table
 */
symbol_t *get_symbol_table(symbol_table_t *symbol_table, char *id)
{
    int table_index, char_sum=0, i;
    symbol_t *sym;
    
    for(i=0; i < strlen(id); i++)
        char_sum += (int) id[i];

    table_index = char_sum % symbol_table->size;
    sym = &symbol_table->symbols[table_index];

    //for hash table collisions search the linked list of collisions
    while(sym && sym->initialized)
    {
        if(!strcmp(sym->id, id))
            return sym;
        sym = sym->next;
    }

    return NULL;
}



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

    int log10=0, copy=a>0?a:-a;
    char *str;

    if(!a)
        return strdup("0");

    while(copy > 0)
    {
        copy /= 10;
        log10++;
    }

    log10 = a > 0 ? log10 : log10 + 1;
    str = malloc(sizeof(char) * log10);
    str[log10] = '\0';
    log10--;

    if(a>0)
    {
        while(log10 >= 0)
        {
            str[log10] = (char) (a%10+48);
            a /= 10;
            log10--;
        }
    }
    else
    {
        a = -a;
        while(log10 >= 1)
        {
            str[log10] = (char) (a%10+48);
            a /= 10;
            log10--;
        }
        str[log10] = '-';
    }

    return str;
}
