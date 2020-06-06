#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

/*
 * attempts to parse a series of tokens until success or discovers
 * that the word does not belong to the grammar
 */
int parse(char **pos)
{
    token *result;
    while(1)
    {
        result = scan(pos);
        if(!result)
            break;
        printf("(%s, %s)\n", result->token, result->attribute);
    }
    return 1;
}

/*
 * Check the posted ansi C yacc grammar and convert it so that it doesn't have
 * any left recrusion so that you can just perform recursive descent.
 */
int parse_expression(char **pos){return 0;}
