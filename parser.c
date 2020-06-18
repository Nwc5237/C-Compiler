#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int parse(char **pos)
{
    parse_expression(pos);
    return 1;
}

int parse_expression(char **pos)
{
    char **lookahead = malloc(sizeof(char *));
    char **copy = malloc(sizeof(char *));
    token *tok;

    *lookahead = *pos;
    tok = malloc(sizeof(token));

    if(!parse_factor(lookahead))
        return 0;

    while(1)
    {
        *copy = *lookahead;
        tok = scan(lookahead);

        if(!tok)
        {
            *lookahead = *copy;
            break;
        }

        //if the token is not a binary operator break
        if(tok && strcmp(tok->token, "+") &&
                  strcmp(tok->token, "-") &&
                  strcmp(tok->token, "*") &&
                  strcmp(tok->token, "/"))
        {
            *lookahead = *copy;
            break;
        }

        if(!parse_factor(lookahead))
        {
            *lookahead = *copy;
            break;
        }
    }

    *pos = *lookahead;
    return 1;
}

int parse_factor(char **pos)
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
            if(!parse_expression(lookahead))
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

int parse_binary_op(char **pos)
{
}

int parse_unary_op(char **pos)
{
}
