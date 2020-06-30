#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3AC_interpreter.h"
#include "lexer.h"
#include "utilities.h"

token_t *lex(char **pos)
{
    char *letters = "abcdefghijklmnopqrstuvwxyz";
    char *digits = "0123456789";
    char *forward, *lexeme_begin;
    forward = lexeme_begin = *pos;
    token_t *token = malloc(sizeof(token_t));

    while(in(*forward, " \t\n"))
    {
        lexeme_begin++;
        forward++;
    }

    if(*forward == ';')
    {
        forward++;
        token->token = strdup("EOS");
        token->attribute = strdup(";");
        *pos = forward;
        return token;
    }

    //assignment operator
    if(*forward == ':')
    {
        forward++;
        if(*forward == '=')
        {
            forward++;
            token->token = strdup("assignment");
            token->attribute = strdup(":=");
            *pos = forward;
            return token;
        }
    }

    //scanning for registers which are: 't' digit+
    if(*forward == 't')
    {
        forward++;
        if(in(*forward, digits))
        {
            while(in(*forward, digits))
                forward++;
            token->token = strdup("register");
            token->attribute = strndup(lexeme_begin,
                    (size_t)(forward-lexeme_begin));
            *pos = forward;
            return token;
        }
    }

    //labels and operations
    *forward = *lexeme_begin;
    if(in(*forward, letters))
    {
        forward++;
        while(in(*forward, letters) || in(*forward, digits))
        {
            forward++;
        }
    }

    if(*pos == forward)
        return NULL;

    if(*forward == ':')
        token->token = strdup("label");
    else
        token->token = strdup("op");

    *pos = forward;
    token->attribute = strndup(lexeme_begin, (size_t)(forward-lexeme_begin));
    return token;
}

/*
 * returns the register referenced by token, and if it is not
 * currently initialized in the register file, it initializes
 * it and then returns the fresh register
 */
reg_t *get_reg(reg_t *reg_file, token_t *search)
{
    reg_t *cur, *prev;

    cur = reg_file;
    while(cur)
    {
        if(!strcmp(search->attribute, cur->name))
            return cur;
        
        prev = cur;
        cur = cur->next;
    }
    
    cur = malloc(sizeof(token_t));
    cur->name = search->attribute;
    cur->val = 0;
    prev->next = cur;
    return cur;
}

/*
 * performs an operation on registers
 */
void execute_op(reg_t *reg_file, token_t *destination, token_t *op, token_t *source1, token_t *source2)
{
    reg_t *dest, *src1, *src2;
    
    dest = get_reg(reg_file, destination);
    src1 = get_reg(reg_file, source1);
    src2 = get_reg(reg_file, source2);

    //just check which of all the ops we're performing
    if(!strcmp(op->attribute, "add"))
    {
        dest->val = itoa(atoi(src1->val) + atoi(src2->val));
        return;
    }
    else if(!strcmp(op->attribute, "sub"))
    {
        dest->val = itoa(atoi(src1->val) - atoi(src2->val));
        return;
    }
    else if(!strcmp(op->attribute, "mult"))
    {
        dest->val = itoa(atoi(src1->val) * atoi(src2->val));
        return;
    }
    else if(!strcmp(op->attribute, "div"))
    {
        dest->val = itoa(atoi(src1->val) / atoi(src2->val));
        return;
    }
    else if(!strcmp(op->attribute, "mod"))
    {
        dest->val = itoa(atoi(src1->val) % atoi(src2->val));
        return;
    }
    else
    {
        printf("Op %s is undefined.\n", op->attribute);
        exit(0);
    }
    //else if(!strcmp(op->attribute, ""))
}

//simple grammar, statement -> register := op register register;
int parse_statement(char **pos, reg_t *reg_file)
{
    //ass, lol
    token_t *print, *destination, *ass, *op, *source1, *source2, *eos;
    reg_t *reg;
    char *lookahead = *pos;

    print = lex(&lookahead);
    if(print && !strcmp(print->attribute, "print"))
    {
        source1 = lex(&lookahead);
        if(source1 && !strcmp(source1->token, "register"))
        {
            eos = lex(&lookahead);
            if(eos && !strcmp(eos->token, "EOS"))
            {
                reg = get_reg(reg_file, source1);
                printf("%s=%d\n", reg->name, reg->val);
                *pos = lookahead;
                return 1;
            }
        }
        printf("Error in print statement");
    }

    lookahead = *pos;
    destination = lex(&lookahead);
    if(destination && !strcmp(destination->token, "register"))
    {
        ass = lex(&lookahead);
        if(ass && !strcmp(ass->token, "assignment"))
        {
            op = lex(&lookahead);
            if(op && !strcmp(op->token, "op"))
            {
                source1 = lex(&lookahead);
                if(source1 && !strcmp(source1->token, "register"))
                {
                    source2 = lex(&lookahead);
                    if(source2 && !strcmp(source2->token, "register"))
                    {
                        eos = lex(&lookahead);
                        //EOS (end of statement) aka ";"
                        if(eos && !strcmp(eos->token, "EOS"))
                        {
                            execute_op(reg_file, destination, op, source1, source2);
                            *pos = lookahead;
                            return 1;
                        }
                    }
                }
            }
            source1 = op;
            if(source1 && !strcmp(source1->token, "INT"))
            {
                eos = lex(&lookahead);
                if(eos && !strcmp(eos->token, "EOS"))
                {
                    reg = get_reg(reg_file, source1);
                    reg->val = source1->attribute;
                }
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char *code = argv[1];
    char *pos = code;
    token_t *tok;
    reg_t *reg_file = NULL;

    // statements of form reg := op a b;
    while(parse_statement(&pos, reg_file))
        printf("gooch\n");

    //pass in a new register file and
    //create a memory for the runtime environment
}
