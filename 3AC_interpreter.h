typedef struct reg_t
{
    char *name;
    char *val;
    struct reg_t *next;
} reg_t;

typedef struct token_t
{
    char *token;
    char *attribute;
} token_t;

#ifndef LEX_H_
#define LEX_H_
token_t *lex(char **pos);
#endif // LEX_H_

#ifndef PARSE_STATEMENT_H_
#define PARSE_STATEMENT_H_
int parse_statement(char **pos, reg_t *reg_file);
#endif // PARSE_STATEMENT_H_
