#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

/*
 * attempts to parse a series of tokens until success or discovers
 * that the word does not belong to the grammar. Prints out the
 * amount parsed
 */
int parse(char **pos)
{
    char *start = *pos;
    long i;

    if(!parse_expression(pos))
        printf("Parse failed somewhere.\n");

    //print what was parsed
    printf("Parsed:\n");
    for(i=0; i < (long)(*pos-start); i++)
        printf("%c", start[i]);
    printf("\n");
}

/*
 * Parses expr's according to the production
 * expr -> term term_tail
 */
int parse_expression(char **pos)
{
    char **lookahead;
    lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    
    if(parse_term(lookahead))
    {
        if(parse_term_tail(lookahead))
        {
            *pos = *lookahead;
            return 1;
        }
    }

    return 0;
}

/*
 * Parses term's according to the production
 * term -> factor factor_tail
 */
int parse_term(char **pos)
{
    char **lookahead;
    lookahead = malloc(sizeof(char *));
    *lookahead = *pos;

    if(parse_factor(lookahead))
    {
        if(parse_factor_tail(lookahead))
        {
            *pos = *lookahead;
            return 1;
        }
    }

    return 0;
}

/*
 * Parses term_tail's according to the production
 * term_tail -> add_op term term_tail | epsilon (the empty string)
 */
int parse_term_tail(char **pos)
{
    char **lookahead;
    lookahead = malloc(sizeof(char *));
    *lookahead = *pos;

    if(parse_add_op(lookahead))
    {
        if(parse_term(lookahead))
        {
            if(parse_term_tail(lookahead))
            {
                *pos = *lookahead;  
                return 1;
            }
        }
    }

    //reset the lookahead when the first production fails
    *lookahead = *pos;
    if(parse_epsilon(lookahead))
        return 1;
}

/*
 * Parses factors according to the production
 * factor-> (expr) | num.
 * XXX when symbol table is implemented this production
 * should turn into factor -> (expr) | num | id
 */
int parse_factor(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "INT"))
    {
        *pos = *lookahead;
        return 1;
    }

    *lookahead = *pos;
    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "("))
    {
        if(parse_expression(lookahead))
        {
            tok = scan(lookahead);
            if(tok && !strcmp(tok->token, ")"))
            {
                *pos = *lookahead;
                return 1;
            }
        }
    }

    return 0;
}

/*
 * Parses factor_tail's according to the production
 * factor_tail -> mult_op factor factor_tail | epsilon
 */
int parse_factor_tail(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;

    if(parse_mult_op(lookahead))
    {
        if(parse_factor(lookahead))
        {
            if(parse_factor_tail(lookahead))
            {
                *pos = *lookahead;
                return 1;
            }
        }
    }

    *lookahead = *pos;
    if(parse_epsilon(lookahead))
        return 1;
}

/*
 * Parses mult_op's according to the production
 * mult_op -> * | /
 */
int parse_mult_op(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    
    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "*"))
    {
        *pos = *lookahead;
        return 1;
    }
    else if(tok && !strcmp(tok->token, "/"))
    {
        *pos = *lookahead;
        return 1;
    }
    else
        return 0;
}

/*
 * Parses add_op's according to the production
 * add_op -> + | -
 */
int parse_add_op(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "+"))
    {
        *pos = *lookahead;
        return 1;
    }
    else if(tok && !strcmp(tok->token, "-"))
    {
        *pos = *lookahead;
        return 1;
    }
    else
        return 0;
}

/*
 * Checks for empty string productions which are always true.
 * It would be slightly more efficient to use 1 instead of
 * calling this function, but I want to see how much it actually
 * slows down parsing bigger files (which i suspect won't be much)
 * because I think it makes the code more readable and easily understood
 */
int parse_epsilon(char **pos)
{
    return 1;
}
