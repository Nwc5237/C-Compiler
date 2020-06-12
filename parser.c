#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

/*
 * Either build the parse tree explicitly and do a postorder traversal, or
 * have each function return a production struct which serves the purpose of
 * returning null if the production fails, and returns the value otherwise
 * XXX right now im just going for it. I need to take into account the add_op
 * and mult_op's and can probably just sent that back in the same atttibute
 * struct to the calling function to let it know which operation to perform
 */

/*
 * attempts to parse a series of tokens until success or discovers
 * that the word does not belong to the grammar. Prints out the
 * amount parsed
 */
int parse(char **pos)
{
    char *start = *pos;
    long i;
    attribute *att = malloc(sizeof(attribute));

    att = parse_expression(pos);
    if(att)
    {
        for(i=0; i < (long)(*pos-start); i++)
            printf("%c", start[i]);
        printf("=%d\n", att->INT);
    }
    else
        printf("Failure you bum\n");

    //print what was parsed
    /*
    printf("Parsed:\n");
    for(i=0; i < (long)(*pos-start); i++)
        printf("%c", start[i]);
    printf("\n");
    */

    return 0; //figure out something better here
}
/*
attribute *parse_assignment(char **pos)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    token *tok;
    attribute *att = malloc(sizeof(attribute));

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "ID"))
    {
        tok = scan(lookahead);
        if(tok && !strcmp(tok->token, "="))
        {
            if(parse_expression(lookahead))
            {
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
*/
/*
 * Parses expr's according to the production
 * expr -> term term_tail
 */
attribute *parse_expression(char **pos)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));
    attribute *term = malloc(sizeof(attribute));
    attribute *term_tail = malloc(sizeof(attribute));
    
    term = parse_term(lookahead);
    if(term)
    {
        term_tail = parse_term_tail(lookahead);
        if(term_tail)
        {
            *pos = *lookahead;
            
            //the tail contains the operator in the productions
            if(!strcmp(term_tail->add_op, "+"))
                att->INT = term->INT + term_tail->INT;
            else if(!strcmp(term_tail->add_op, "-"))
                att->INT = term->INT - term_tail->INT;
            else
            {
                printf("Error in parse_expression\n");
                exit(0);
            }
            free(term);
            free(term_tail);
            return att;
        }
    }

    free(term);
    free(term_tail);
    return NULL;
}

/*
 * Parses term's according to the production
 * term -> factor factor_tail
 */
attribute *parse_term(char **pos)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));
    attribute *factor = malloc(sizeof(attribute));
    attribute *factor_tail = malloc(sizeof(attribute));


    factor = parse_factor(lookahead);
    if(factor)
    {
        factor_tail = parse_factor_tail(lookahead);
        if(factor_tail)
        {
            *pos = *lookahead;

            if(!strcmp(factor_tail->mult_op, "*"))
                att->INT = factor->INT * factor_tail->INT;
            else if(!strcmp(factor_tail->mult_op, "/"))
                att->INT = factor->INT / factor_tail->INT;
            else
            {
                printf("Error in parse term\n");
                exit(0);
            }
            free(factor);
            free(factor_tail);
            return att;
            //XXX for epsilon productions you could probably just use identity (*1 or +0) to get around awkward if's
        }
    }

    free(factor);
    free(factor_tail);
    return NULL;
}

/*
 * Parses term_tail's according to the production
 * term_tail -> add_op term term_tail | epsilon (the empty string)
 */
attribute *parse_term_tail(char **pos)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));
    attribute *add_op = malloc(sizeof(attribute));
    attribute *term = malloc(sizeof(attribute));
    attribute *term_tail = malloc(sizeof(attribute));

    add_op = parse_add_op(lookahead);
    if(add_op)
    {
        term = parse_term(lookahead);
        if(term)
        {
            term_tail = parse_term_tail(lookahead);
            if(term_tail)
            {
                *pos = *lookahead;
                att->add_op = strdup(add_op->add_op);
                if(!strcmp(term_tail->add_op, "+"))
                    att->INT = term->INT + term_tail->INT;
                else if(!strcmp(term_tail->add_op, "-"))
                    att->INT = term->INT - term_tail->INT;
                else
                {
                    printf("Error in parse_term_tail\n");
                    exit(0);
                }
                free(add_op);
                free(term);
                free(term_tail);
                return att;
            }
        }
    }

    //reset the lookahead when the first production fails XXX liker here you could return with att.INT=0 probably
    *lookahead = *pos;
    if(parse_epsilon(lookahead))
    {
        free(add_op);
        free(term);
        free(term_tail);
        att->add_op = strdup("+");
        att->INT = 0;
        return att;
    }
}

/*
 * Parses factors according to the production
 * factor-> (expr) | num.
 * XXX when symbol table is implemented this production
 * should turn into factor -> (expr) | num | id
 */
attribute *parse_factor(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "INT"))
    {
        *pos = *lookahead;
        att->INT= atoi(tok->attribute);
        return att;
    }

    *lookahead = *pos;
    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "("))
    {
        att = parse_expression(lookahead);
        if(att)
        {
            tok = scan(lookahead);
            if(tok && !strcmp(tok->token, ")"))
            {
                *pos = *lookahead;
                return att;
            }
        }
    }

    return NULL;
}

/*
 * Parses factor_tail's according to the production
 * factor_tail -> mult_op factor factor_tail | epsilon
 */
attribute *parse_factor_tail(char **pos)
{
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));
    attribute *mult_op = malloc(sizeof(attribute));
    attribute *factor = malloc(sizeof(attribute));
    attribute *factor_tail = malloc(sizeof(attribute));

    mult_op = parse_mult_op(lookahead);
    if(mult_op)
    {
        factor = parse_factor(lookahead);
        if(factor)
        {
            factor_tail = parse_factor_tail(lookahead);
            if(factor_tail)
            {
                *pos = *lookahead;
                att->mult_op = strdup(mult_op->mult_op);
                if(!strcmp(factor_tail->mult_op, "*"))
                    att->INT = factor->INT * factor_tail->INT;
                else if(!strcmp(factor_tail->mult_op, "/"))
                    att->INT = factor->INT / factor_tail->INT;
                else
                {
                    printf("Error in parse_factor_tail\n");
                    exit(0);
                }
                free(mult_op);
                free(factor);
                free(factor_tail);
                return att;
            }
        }
    }

    *lookahead = *pos;
    if(parse_epsilon(lookahead))
    {
        free(mult_op);
        free(factor);
        free(factor_tail);
        att->mult_op = strdup("*");
        att->INT = 1;
    }
}

/*
 * Parses mult_op's according to the production
 * mult_op -> * | /
 */
attribute *parse_mult_op(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));
    
    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "*"))
    {
        *pos = *lookahead;
        att->mult_op = strdup("*");
        return att;
    }
    else if(tok && !strcmp(tok->token, "/"))
    {
        *pos = *lookahead;
        att->mult_op = strdup("/");
        return att;
    }
    else
        return NULL;
}

/*
 * Parses add_op's according to the production
 * add_op -> + | -
 */
attribute *parse_add_op(char **pos)
{
    token *tok;
    char **lookahead = malloc(sizeof(char *));
    *lookahead = *pos;
    attribute *att = malloc(sizeof(attribute));

    tok = scan(lookahead);
    if(tok && !strcmp(tok->token, "+"))
    {
        *pos = *lookahead;
        att->add_op = strdup("+");
        return att;
    }
    else if(tok && !strcmp(tok->token, "-"))
    {
        *pos = *lookahead;
        att->add_op = strdup("-");
        return att;
    }
    else
        return NULL;
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
