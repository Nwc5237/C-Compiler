#ifndef ELEMENT_H_
#define ELEMENT_H_
typedef struct element
{
    char *data;
    struct element *next;
    struct element *prev;
} element;
#endif // ELEMENT_H_

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
