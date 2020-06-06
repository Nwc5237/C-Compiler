/*#ifndef TOKEN_H_
#define TOKEN_H_
typedef struct token
{
    char *token;
    char *attribute;
} token;
#endif // TOKEN_H_

extern const char *letters;
extern const char *numbers;
extern const char *whitespace;
extern const char *delimeters;

#ifndef IN_H_
#define IN_H_
int in(char c, const char *set);
#endif // IN_H_

#ifndef ACCEPT_KEYWORD_H_
#define ACCEPT_KEYWORD_H_
token *accept_keyword(char **lexeme_begin);
#endif // ACCEPT_KEYWORD_H_

#ifndef ACCEPT_IDENTIFIER_H_
#define ACCEPT_IDENTIFIER_H_
token *accept_identifier(char **lexeme_begin);
#endif // ACCEPT_IDENTIFIER_H_

#ifndef ACCEPT_NUMBER_H_
#define ACCEPT_NUMBER_H_
token *accept_number(char **lexeme_begin);
#endif // ACCEPT_NUMBER_H_

#ifndef ACCEPT_RELOP_H_
#define ACCEPT_RELOP_H_
token *accept_relop(char **lexeme_begin);
#endif // ACCEPT_RELOP_H_

#ifndef ACCEPT_OPERATOR_H_
#define ACCEPT_OPERATOR_H_
token *accept_operator(char **lexeme_begin);
#endif // ACCEPT_OPERATOR_H_

#ifndef ACCEPT_LOGICAL_H_
#define ACCEPT_LOGICAL_H_
token *accept_logical(char **lexeme_begin);
#endif // ACCEPT_LOGICAL_H_

#ifndef SCAN_H_
#define SCAN_H_
token *scan(char **pos);
#endif // SCAN_H_*/

//XXX
#ifndef TOKEN_H_
#define TOKEN_H_
typedef struct token
{
    char *token;
    char *attribute;
} token;
#endif // TOKEN_H_

//extern const char *letters;
//extern const char *numbers;
//extern const char *whitespace;
//extern const char *delimeters;

int in(char c, const char *set);
token *accept_keyword(char **lexeme_begin);
token *accept_identifier(char **lexeme_begin);
token *accept_number(char **lexeme_begin);
token *accept_relop(char **lexeme_begin);
token *accept_operator(char **lexeme_begin);
token *accept_logical(char **lexeme_begin);
token *scan(char **pos);
