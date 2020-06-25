#ifndef ELEMENT_H_
#define ELEMENT_H_
typedef struct element
{
    char *data;
    struct element *next;
    struct element *prev;
} element;
#endif // ELEMENT_H_

#ifndef TYPE_UNION_T_H_
#define TYPE_UNION_T_H_
typedef union type_union_t
{
    char c;
    short s;
    int i;
    long l;

    float f;
    double d;

    void *ptr;
    char *str;
} type_union_t;
#endif // TYPE_UNION_T_H_

#ifndef SYMBOL_T_H_
#define SYMBOL_T_H_
typedef struct symbol_t
{
    int initialized;
    char *type;
    char *id;
    type_union_t val;
    struct symbol_t *next;
    struct symbol_t *prev;
} symbol_t;
#endif // SYMBOL_T_H_

#ifndef SYMBOL_TABLE_T_H_
#define SYMBOL_TABLE_T_H_
typedef struct symbol_table_t
{
    symbol_t *symbols;
    int size;
} symbol_table_t;
#endif // SYMBOL_TABLE_T_H_

#ifndef INIT_SYMBOL_TABLE_H_
#define INIT_SYMBOL_TABLE_H_
symbol_table_t *init_symbol_table(int size);
#endif // INIT_SYMBOL_TABlE_H_

#ifndef GET_SYMBOL_TABLE_H_
#define GET_SYMBOL_TABLE_H_
symbol_t *get_symbol_table(symbol_table_t *symbol_table, char *id);
#endif // GET_SYMBOL_TABLE_H_

#ifndef INSERT_SYMBOL_TABLE_H_
#define INSERT_SYMBOL_TABLE_H_
void insert_symbol_table(symbol_table_t *symbol_table, char *id, type_union_t val);
#endif // INSERT_SYMBOL_TABLE_H_

#ifndef STACK_H_
#define STACK_H_
typedef struct stack
{
    element *top;
} stack;
#endif // STACK_H_

#ifndef PUSH_H_
#define PUSH_H_
void push(stack *s, element *e);
#endif // PUSH_H_

#ifndef POP_H_
#define POP_H_
element *pop(stack *s);
#endif // POP_H_

#ifndef PEEK_H_
#define PEEK_H_
element *peek(stack *s);
#endif // PEEK_H_

#ifndef IS_EMPTY_H_
#define IS_EMPTY_H_
int is_empty(stack *s);
#endif // IS_EMPTY_H

#ifndef INIT_ELEMENT_H_
#define INIT_ELEMENT_H_
element *init_element(char *data);
#endif // INIT_ELEMENT_H_

#ifndef INIT_STACK_H_
#define INIT_STACK_H_
stack *init_stack();
#endif // INIT_STACK_H_

#ifndef INIT_ITOA_H_
#define INIT_ITOA_H_
char *itoa(int a);
#endif // INIT_ITOA_H_

