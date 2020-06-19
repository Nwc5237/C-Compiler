#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "data_structures.h"
#include "parser.h"

int parse(char **pos)
{
    stack *operators=malloc(sizeof(stack)), *operands=malloc(sizeof(stack));
    parse_expression(pos, operators, operands);
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
        /*tok = scan(lookahead);

        if(!tok)
        {
            *lookahead = *copy;
            break;
        }
*/
        //if the token is not a binary operator break
        if(!parse_binary_op(lookahead, operators, operands))
        {
            *lookahead = *copy;
            break;
        }

        if(!parse_factor(lookahead, operators, operands))
        {
            *lookahead = *copy;
            break;
        }
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
            goto pass;

        if(!strcmp(tok->token, "("))
        {
            if(!parse_expression(lookahead, operators, operands))
                return 0;

            tok = scan(lookahead);
            if(tok && !strcmp(tok->token, ")"))
                goto pass;
        }
    }

    return 0;

//I'm  a bad boy for using a goto statement. Think for yourselves people.
pass:
    *pos = *lookahead;
    return 1;
}

int parse_binary_op(char **pos, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    token *tok;

    tok = scan(lookahead);
    if(tok &&
            (!strcmp(tok->token, "+") ||
             !strcmp(tok->token, "-") ||
             !strcmp(tok->token, "*") ||
             !strcmp(tok->token, "/")))
    {
        *pos = *lookahead;
        return 1;
    }

    return 0;

}

int parse_unary_op(char **pos, stack *operators, stack *operands)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    token *tok;

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "-"))
    {
        *pos = *lookahead;
        return 1;
    }

    return 0;
}
