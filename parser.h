#include "lexer.h"
#include "data_structures.h"

#ifndef EXPECT_H_
#define EXPECT_H_
void expect(char *expected, char **pos);
#endif // EXPECT_H_

#ifndef PARSE_H_
#define PARSE_H_
int parse(char **pos);
#endif // PARSE_H_

#ifndef PARSE_EXPRESSION_H_
#define PARSE_EXPRESSION_H_
int parse_expression(char **pos, stack *operators, stack *operands);
#endif // PARSE_EXPRESSION_H_

#ifndef PARSE_FACTOR_H_
#define PARSE_FACTOR_H_
int parse_factor(char **pos, stack *operators, stack *operands);
#endif // PARSE_FACTOR_H_

#ifndef PARSE_BINARY_OP_H_
#define PARSE_BINARY_OP_H_
int parse_binary_op(char **pos, stack *operators, stack *operands);
#endif // PARSE_BINARY_OP_H_

#ifndef PARSE_UNARY_OP_H_
#define PARSE_UNARY_OP_H_
int parse_unary_op(char **pos, stack *operators, stack *operands);
#endif // PARSE_UNARY_OP_H_

