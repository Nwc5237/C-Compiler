#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "data_structures.h"
#include "parser.h"

//Assumes EOF token ($) has already been placed at the end of the input
int parse(char **pos)
{
    stack *operators=malloc(sizeof(stack)), *operands=malloc(sizeof(stack));
    push(operators, init_element("sentinel"));
    parse_expression(pos, operators, operands);
    printf("result: %s\n", pop(operands)->data);
    return 1;
}

int parse_expression(char **pos, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    char **copy = malloc(sizeof(char *));
    token *tok;

    *lookahead = *pos;
    tok = malloc(sizeof(token));

    if(!parse_factor(lookahead, operators, operands))
        return 0;

    while(1)
    {
        *copy = *lookahead;
        if(!parse_binary_op(lookahead, operators, operands))
        {
            *lookahead = *copy;
            break;
        }

        if(!parse_factor(lookahead, operators, operands))
        {
            printf("Error parsing: binary operator missing operand\n");
            exit(0);
        }
    }
    
    if(!parse_expression_end(lookahead, operators, operands))
    {
        *lookahead = *copy;
        printf("Error before parsing EOF while parsing expressions\n");
        exit(0);
    }

    *pos = *lookahead;
    return 1;
}

int parse_factor(char **pos, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    token *tok;

    *lookahead = *pos;
    tok = malloc(sizeof(token));

    tok = scan(lookahead);
    if(tok)
    {
        if(!strcmp(tok->token, "INT"))
        {
             push(operands, init_element(tok->attribute));
             goto pass;
        }

        if(!strcmp(tok->token, "("))
        {
            push(operators, init_element("sentinel"));
            if(!parse_expression(lookahead, operators, operands))
                return 0;

            tok = scan(lookahead);
            if(tok && !strcmp(tok->token, ")"))
            {
                pop(operators);
                goto pass;
            }
        }

        if(!strcmp(tok->token, "-") || !strcmp(tok->token, "+"))
        {
            if(parse_unary_op(lookahead, operators, operands, tok->token))
                goto pass;
        }
    }

    return 0;

//I'm  a bad boy for using a goto statement. Think for yourselves people.
pass:
    *pos = *lookahead;
    return 1;
}

//XXX without parentheses:
int parse_binary_op(char **pos, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    token *tok;
    element *top;
    int v1, v2;

    tok = scan(lookahead);
    if(tok &&
            (!strcmp(tok->token, "+") ||
             !strcmp(tok->token, "-")))
    {    
        top = peek(operators);
 
        //when theres no other operator we need to wait for another operand
        //so we just push the current operator
        if(!strcmp(top->data, "sentinel"))
        {
            push(operators, init_element(tok->token));
            *pos = *lookahead;
            return 1;
        }
        
        //while theres higher precednece operators on the stack perform them
        while(top &&
                (!strcmp(top->data, "*") || !strcmp(top->data, "/")))
        {
            pop(operators);
            v2 = atoi(pop(operands)->data);
            v1 = atoi(pop(operands)->data);

            !strcmp(top->data, "*") ?
                push(operands, init_element(itoa(v1*v2))) :
                push(operands, init_element(itoa(v1/v2)));

            top = peek(operators);
        }

        if(top &&
                (!strcmp(top->data, "+") || !strcmp(top->data, "-")))
        {
            pop(operators);
            v2 = atoi(pop(operands)->data);
            v1 = atoi(pop(operands)->data);

            !strcmp(top->data, "+") ?
                push(operands, init_element(itoa(v1+v2))) :
                push(operands, init_element(itoa(v1-v2)));

        }

        push(operators, init_element(tok->token));
        *pos = *lookahead;
        return 1;
    }


    if(tok &&
            !strcmp(tok->token, "*") ||
            !strcmp(tok->token, "/"))
    {
        top = peek(operators);

        if(!strcmp(top->data, "sentinel"))//!top)
        {
            push(operators, init_element(tok->token));
            *pos = *lookahead;
            return 1;
        }
        
        //perform greater or equal ops first
        while(top &&
                (!strcmp(top->data, "*") || !strcmp(top->data, "/")))
        {
            pop(operators);
            v2 = atoi(pop(operands)->data);
            v1 = atoi(pop(operands)->data);

            !strcmp(top->data, "*") ?
                push(operands, init_element(itoa(v1*v2))) :
                push(operands, init_element(itoa(v1/v2)));

            top = peek(operators);
        }

        push(operators, init_element(tok->token));
        *pos = *lookahead;
        return 1;
    }

    return 0;

}

int parse_unary_op(char **pos, stack *operators, stack *operands, char *prefix)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    token *tok;
    element *top;

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "INT"))
    {
        top = peek(operators);
        if(top &&
                (!strcmp(top->data, "*") ||
                 !strcmp(top->data, "/") ||
                 !strcmp(top->data, "sentinel")))
        {
            if(!strcmp(prefix, "-"))
                push(operands, init_element(itoa(-atoi(tok->attribute))));
            else
                push(operands, init_element(tok->attribute));
            
            *pos = *lookahead;
            return 1;
        }
    }

    //here could parse expressions like -(factor)

    return 0;
}


int parse_expression_end(char **pos, stack *operators, stack *operands)
{
    int v1, v2;
    token *tok;
    char **lookahead = malloc(sizeof(char *));

    *lookahead = *pos;

    //While we haven't popped to the sentinel
    while(strcmp(peek(operators)->data, "sentinel"))//!is_empty(operators))
    {
        v2 = atoi(pop(operands)->data);
        v1 = atoi(pop(operands)->data);

        char op = *pop(operators)->data;
        switch(op)
        {
            case '+':
                push(operands, init_element(itoa(v1+v2)));
                *pos = *lookahead;
                break;

            case '-':
                push(operands, init_element(itoa(v1-v2)));
                *pos = *lookahead;
                break;
            
            case '*':
                push(operands, init_element(itoa(v1*v2)));
                *pos = *lookahead;
                break;
            
            case '/':
                push(operands, init_element(itoa(v1/v2)));
                *pos = *lookahead;
                break;
            
            default:
                printf("Error in parse EOF\n");
                exit(0);
        }
    }
    return 1;
}
