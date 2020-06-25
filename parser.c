#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "utilities.h"
#include "parser.h"

/*
 * Directs the entire parse. Using recursive descent and shunting yard
 * algorithms. Currently parsing mathematical expressions.
 */
int parse(char **pos)
{
    int symbol_table_size = 512;
    symbol_table_t *symbol_table = init_symbol_table(symbol_table_size);
    while(parse_assignment(pos, symbol_table));

    //stuff
    stack *operators=malloc(sizeof(stack)), *operands=malloc(sizeof(stack));
    push(operators, init_element("sentinel"));
    if(parse_expression(pos, symbol_table, operators, operands))
        printf("Result: %s\n", pop(operands)->data);
    else
        printf("Parse failed :(\n");
    
    return 1;
}

/*
 * Initializes and places values in symbol table
 * assignment -> id = val;
 */
int parse_assignment(char **pos, symbol_table_t *symbol_table)
{
    char **lookahead = malloc(sizeof(char *));
    char *id;
    type_union_t val;
    token *tok;
    symbol_t *sym;
    stack *operators=malloc(sizeof(stack)), *operands=malloc(sizeof(stack));

    *lookahead = *pos;
    tok = malloc(sizeof(token));

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "ID"))
    {
        id = strdup(tok->attribute);
        tok = scan(lookahead);
        if(tok && !strcmp(tok->token, "="))
        {
            push(operators, init_element("sentinel"));
            if(parse_expression(lookahead, symbol_table, operators, operands))
            {
                val.i = atoi(pop(operands)->data);
                if((sym = get_symbol_table(symbol_table, id)))
                    sym->val = val;
                else
                    insert_symbol_table(symbol_table, id, val); //Add the type here when you implement type checking

                tok = scan(lookahead);
                if(tok && !strcmp(tok->token, ";"))
                {
                    *pos = *lookahead;
                    return 1;
                }
            }
        }
    }

    return 0;
    
}


/*
 * expr -> expr {binary_op expr}
 */
int parse_expression(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    char **copy = malloc(sizeof(char *));
    token *tok;

    *lookahead = *pos;
    tok = malloc(sizeof(token));

    if(!parse_factor(lookahead, symbol_table, operators, operands))
        return 0;

    while(1)
    {
        *copy = *lookahead;
        if(!parse_binary_op(lookahead, symbol_table, operators, operands))
        {
            *lookahead = *copy;
            break;
        }

        if(!parse_factor(lookahead, symbol_table, operators, operands))
        {
            printf("Error parsing: missing variable or integer literal.\n");
            exit(0);
        }
    }
    
    if(!parse_expression_end(lookahead, operators, operands))
    {
        *lookahead = *copy;
        printf("Error parsing: expression could not be evaluated.\n");
        exit(0);
    }

    *pos = *lookahead;
    return 1;
}


/*
 * factor -> int | (expr) | unary_op factor
 */
int parse_factor(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    token *tok;
    symbol_t *sym;

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
            if(!parse_expression(lookahead, symbol_table, operators, operands))
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
            if(parse_unary_op(lookahead, symbol_table, operators, operands, tok->token))
                goto pass;
        }

        if(!strcmp(tok->token, "ID"))
        {
            sym = get_symbol_table(symbol_table, tok->attribute);
            if(sym)
            {
                push(operands, init_element(itoa(sym->val.i)));
                goto pass;
            }
            else
            {
                printf("Error parsing: uninitialized variable\n");
                exit(0);
            }
        }
    }

    return 0;

//Using a goto statement like a bad boy
pass:
    *pos = *lookahead;
    return 1;
}

/*
 * binary_op -> + | - | * | /
 */
int parse_binary_op(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands)
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
 
        //push current operator while waiting for next operand to evaluate
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

        if(!strcmp(top->data, "sentinel"))
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

/*
 * unary_op -> + | -
 */
int parse_unary_op(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands, char *prefix)
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

    return 0;
}

/*
 * When we reach the end of a mathematical expression we probably still have
 * operators and operands waiting on their respective stacks. This function
 * clears and evaluates them.
 */
int parse_expression_end(char **pos, stack *operators, stack *operands)
{
    int v1, v2;
    token *tok;
    char **lookahead = malloc(sizeof(char *));

    *lookahead = *pos;

    while(strcmp(peek(operators)->data, "sentinel"))
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
                printf("Error parsing: expected binary operator +, -, *, or /.\n");
                exit(0);
        }
    }

    return 1;
}
