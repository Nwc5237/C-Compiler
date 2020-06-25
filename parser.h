#include "lexer.h"
#include "utilities.h"

#ifndef PARSE_H_
#define PARSE_H_
int parse(char **pos);
#endif // PARSE_H_

#ifndef PARSE_ASSIGNMENT_H_
#define PARSE_ASSIGNMENT_H_
int parse_assignment(char **pos, symbol_table_t *symbol_table);
#endif // PARSE_ASSIGNMENT_H_

#ifndef PARSE_EXPRESSION_H_
#define PARSE_EXPRESSION_H_
int parse_expression(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands);
#endif // PARSE_EXPRESSION_H_

#ifndef PARSE_FACTOR_H_
#define PARSE_FACTOR_H_
int parse_factor(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands);
#endif // PARSE_FACTOR_H_

#ifndef PARSE_BINARY_OP_H_
#define PARSE_BINARY_OP_H_
int parse_binary_op(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands);
#endif // PARSE_BINARY_OP_H_

#ifndef PARSE_UNARY_OP_H_
#define PARSE_UNARY_OP_H_
int parse_unary_op(char **pos, symbol_table_t *symbol_table, stack *operators, stack *operands, char *prefix);
#endif // PARSE_UNARY_OP_H_

#ifndef PARSE_EXPRESSION_END_H_
#define PARSE_EXPRESSION_END_H_
int parse_expression_end(char **pos, stack *operators, stack *operands);
#endif // PARSE_EXPRESSION_END_H_
