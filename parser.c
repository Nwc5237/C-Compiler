#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

/*
 * attempts to parse a series of tokens until success or discovers
 * that the word does not belong to the grammar. Returns 0 on error
 * and returns 1 on successful parse.
 */
int parse(char **pos)
{
    //check that each production is within the calculator language
    //then add some semantic rule to each production
    token *tok;
    char **cur, **lookahead;
    cur = malloc(sizeof(char *));
    lookahead = malloc(sizeof(char *));
    *cur = *lookahead = *pos;

    //while(1)
    {
        tok = scan(lookahead);

        /*
         * The idea here is to try all of the productions that correspond
         * to first token we see until one of them fully parser. If none
         * work then we return error.
         */
        if(!strcmp(tok->token, "INT"))
        {
            int i = parse_expression(cur);
            printf("parse_experssion(): %d\n", i);
            if(i)
                return 1;
            else
                return 0;
        }
        else if(!strcmp(tok->token, "FLOAT"))
        {
            if(parse_expression(cur))
                return 1;
            else
                return 0;
        }
        else
            return 0;
        //else if(strcmp(tok.token, ""))

    }


    /*token *result;
    while(1)
    {
        result = scan(pos);
        if(!result)
            break;
        printf("(%s, %s)\n", result->token, result->attribute);
    }*/
    return 1;
}

/*
 * Check the posted ansi C yacc grammar and convert it so that it doesn't have
 * any left recrusion so that you can just perform recursive descent.
 */
int parse_expression(char **pos)
{
    token *tok;
    char **cur, **lookahead;
    cur = malloc(sizeof(char *));
    lookahead = malloc(sizeof(char *));
    *cur = *lookahead = *pos;

    int val1, val2;
    while(1)
    {
        tok = scan(lookahead);
        if(!tok)
            break;
        if(!strcmp(tok->token, "INT"))
        {
            val1 = atoi(tok->attribute);
            tok = scan(lookahead);
            if(tok && !strcmp(tok->token, "MATH"))
            {
                if(!strcmp(tok->attribute, "+"))
                {
                    *cur = *lookahead;
                    tok = scan(lookahead);
                    if(tok && !strcmp(tok->token, "INT"))
                    {
                        return val1 + parse_expression(cur);
                    }
                }
                else
                    return 0;
            }
            else if(tok && !strcmp(tok->token, ";"))
                return val1;
            else
                return 0;
        }
    }
    return 0;
}
