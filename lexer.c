#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

//useful sets of characters
const char *letters    = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *numbers    = "0123456789";
const char *whitespace = " \t\n";

//returns 1 if c is a member of set, else return 0
int in(char c, const char *set)
{
    int i = 0;
    while(set[i] != '\0')
    {
        if(c == set[i])
            return 1;
        i++;
    }

    return 0;
}

/*
 * Lex out a couple of the C reserve words. For now teh list includes
 * the following: if, else, while, for, int, float
 */
token *accept_keyword(char **lexeme_begin)
{
    //a switch statement would be a bit of an improvement so maybe try that
    token *keyword_token;
    char *forward;
    char *keywords[] = {"auto", "const", "double", "float", "int", "short",
        "struct", "unsigned", "break", "continue", "else", "for", "long",
        "signed", "switch", "void", "case", "default", "enum", "goto",
        "register", "sizeof", "typedef", "volatile", "char", "do", "extern",
        "if", "return", "static", "union", "while"};
    int i, j, len;
    for(i=0; i<32; i++)
    {
        forward = *lexeme_begin;
        len = strlen(keywords[i]);
        for(j=0; j<len; j++)
        {
            if(keywords[i][j] != *forward)
                break;
            forward++;
        }
        if(j == len)
        {
            if(!in(*forward, letters) && !in(*forward, numbers)
                    && *forward != '_')
            {
                *lexeme_begin = forward;
                keyword_token = (token *) malloc(sizeof(token));
                keyword_token->token = strdup(keywords[i]);
                keyword_token->attribute = strdup("KEYWORD");
                return keyword_token;
            }
        }
    }
    return NULL;
}

/*
 * Recognizes identifiers which are of the form: letter_(letter_|number)*
 */
token *accept_identifier(char **lexeme_begin)
{
    token *id_token;
    char *forward = *lexeme_begin;

    //regular expressions for identifier: letter_(letter_ | number)*
    if(in(**lexeme_begin, letters) || **lexeme_begin == '_')
    {
        
        forward++;
        while(1)
        {
            if(*forward == '_' || in(*forward, letters)
                    || in(*forward, numbers))
                forward++;
            else
                break;
        }

        id_token = (token *) malloc(sizeof(token));
        id_token->token = strdup("ID");
        id_token->attribute = strndup(*lexeme_begin, (size_t)(forward - *lexeme_begin));
        *lexeme_begin = forward;
        
        return id_token;
    }
    return NULL;
}

token *accept_number(char **lexeme_begin)
{
    token *num_token;
    char *forward = *lexeme_begin;

    while(in(*forward, numbers))
    {
        forward++;
    }

    //skipping checking for floats like '123E-45' for now
    if(!in(*forward, letters) && *forward != '.' && *forward != '_')
    {
        num_token = (token *) malloc(sizeof(token));
        num_token->token = strdup("INT");
        num_token->attribute = strndup(*lexeme_begin,
                (size_t)(forward - *lexeme_begin));
        *lexeme_begin = forward;
        return num_token;
    }
    else if(*forward == '.')
    {
        forward++;
        while(in(*forward, numbers))
            forward++;
        if(!in(*forward, letters) && *forward != '_')
        {
            num_token = (token *) malloc(sizeof(token));
            num_token->token = strdup("FLOAT");
            num_token->attribute = strndup(*lexeme_begin,
                    (size_t)(forward - *lexeme_begin));
            *lexeme_begin = forward;
            return num_token;
        }
        return NULL;
    }
    else
        return NULL;
}

/* 
 * recognize relational operators and similar operators such as
 * <, >, <=, >=, ==, and the assignment operator '='
 */
token *accept_relop(char **lexeme_begin)
{
    token *relational_token;
    char *forward = *lexeme_begin;

    switch(*forward)
    {
        case '<':
            forward++;
            if(*forward == '=')
                forward++;
            break;
        case '>':
            forward++;
            if(*forward == '=')
                forward++;
            break;
        case '=':
            forward++;
            if(*forward != '=')
            {
                relational_token = (token *) malloc(sizeof(token));
                relational_token->token = strdup("=");
                *lexeme_begin = forward;
                return relational_token;
            }
            forward++;
            break;
        //this should really never be triggered
        default:
            NULL;
    }
    relational_token = (token *) malloc(sizeof(token));
    relational_token->token = strdup("RELOP");
    relational_token->attribute = strndup(*lexeme_begin,
            (size_t)(forward - *lexeme_begin));
    *lexeme_begin = forward;
    return relational_token;
}

/*
 * Lexes out the assignment operators, arithmetic operators, and
 * increment and decrement operators
 */
token *accept_operator(char **lexeme_begin)
{
    token *op_token;
    char *forward = *lexeme_begin;
    size_t len;

    switch(*forward)
    {
        case '+':
            forward++;
            if(*forward == '=')
            {
                forward++;
                break;
            }
            if(*forward == '+')
            {
                forward++;
                *lexeme_begin = forward;
                op_token = (token *) malloc(sizeof(token));
                op_token->token = strdup("INCREMENT");
                op_token->attribute = strdup("++");
                return op_token;
            }
            break;
        case '-':
            forward++;
            if(*forward == '=')
            {
                forward++;
                break;
            }
            if(*forward == '-')
            {
                forward++;
                *lexeme_begin = forward;
                op_token = (token *) malloc(sizeof(token));
                op_token->token = strdup("DECREMENT");
                op_token->attribute = strdup("--");
                return op_token;
            }
            break;
        case '*':
            forward++;
            if(*forward == '=')
                forward++;
            break;
        case '/':
            forward++;
            if(*forward == '=')
                forward++;
            break;
        case '%':
            forward++;
            if(*forward == '=')
                forward++;
            break;
        //control is never actually passed to default
        default:
            return NULL;
    }

    len = (size_t)(forward - *lexeme_begin);
    op_token = (token *) malloc(sizeof(token));
    if(len == 1)
        op_token->token = strndup(*lexeme_begin, len);
    else
        op_token->token = strdup("ASSIGNMENT_OP");
    
    op_token->attribute = strndup(*lexeme_begin, len);
    *lexeme_begin = forward;

    return op_token;

    //the '=' assignment op is lexed with the relational op's
}

/*
 * returns the logical or bitwise operator found
 */
token *accept_logical(char **lexeme_begin)
{
    token *lb_token = (token *) malloc(sizeof(token));
    char *forward = *lexeme_begin;
    size_t len;

    switch(*forward)
    {
        case '&':
            forward++;
            if(*forward == '&')
                forward++;
            break;
        case '|':
            forward++;
            if(*forward == '|')
                forward++;
            break;
        case '!':
            forward++;
            if(*forward == '=')
                forward++;
            break;
        case '^':
            forward++;
            break;
        case '~':
            forward++;
            break;
        default:
            return NULL;;
    }

    len = (size_t)(forward - *lexeme_begin);
    if(len == 1)
        lb_token->token = strdup("BITWISE_OP");
    else
        lb_token->token = strdup("LOGICAL_OP");

    lb_token->attribute = strndup(*lexeme_begin, len);
    *lexeme_begin = forward;
    return lb_token;
}

/*
 * Scan is basically a big switch statement testing which token we
 * could potentially be parsing. On successful recognition of a token
 * it returns a pointer to a (struct) token, otherwise returns a null
 * pointer.
 */
token *scan(char **pos)
{
    //remove leading whitespace
    while(in(**pos, whitespace))
        (*pos)++;

    token *token_found;
    switch(**pos)
    {
        case 'a' ... 'z':
            token_found = accept_keyword(pos); //if not a null pointer return
            if(token_found)
                return token_found;
        case 'A' ... 'Z':
        case '_':
            token_found = accept_identifier(pos);
            if(token_found)
                return token_found;
            else
                NULL;
            break;

        //search for integers or floaing point numbers
        case '1' ... '9':
        case '0':
        /* 
         * continue the number search and if that fails, it's the struct member
         * operator or ellipsis ("...")
         */
        case '.':
            token_found = accept_number(pos);
            if(token_found)
                return token_found;
            //else
                //return accept_dot(input, pos);
            return NULL;
        
        //relational operators (also catches assignment operator '=')
        case '<':
        case '>':
        case '=':
            token_found = accept_relop(pos);
            if(token_found)
                return token_found;
            return NULL;

        //some delimeters/punctuators
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ',':
        case ':':
        case ';':
            token_found = (token *) malloc(sizeof(token));
            token_found->token = strndup(*pos, 1);
            (*pos)++;
            return token_found;
        
        //lexing &&, ||, !=, ~, ^, !, |, &
        case '&':
        case '|':
        case '!':
        case '~':
        case '^':
            return accept_logical(pos);

        //lexing +=, /=, ++, -- etc
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
           return  accept_operator(pos);

        //EOF token
        case '$':
           token_found = (token *) malloc(sizeof(token));
           token_found->token = strndup(*pos, 1);
           (*pos)++;
           return token_found;

        default:
            return NULL;
    }
}
