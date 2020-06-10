#ifndef PARSE_H_
#define PARSE_H_
int parse(char **pos);
#endif // PARSE_H_

#ifndef PARSE_EXPRESSION_H_
#define PARSE_EXPRESSION_H_
int parse_expression(char **pos);
#endif // PARSE_EXPRESSION_H_

#ifndef PARSE_TERM_H_
#define PARSE_TERM_H_
int parse_term(char **pos);
#endif // PARSE_TERM_H_

#ifndef PARSE_TERM_TAIL_H_
#define PARSE_TERM_TAIL_H_
int parse_term_tail(char **pos);
#endif // PARSE_TERM_TAIL_H_

#ifndef PARSE_FACTOR_H_
#define PARSE_FACTOR_H_
int parse_factor(char **pos);
#endif // PARSE_FACTOR_H_

#ifndef PARSE_FACTOR_TAIL_H_
#define PARSE_FACTOR_TAIL_H_
int parse_factor_tail(char **pos);
#endif // PARSE_FACTOR_TAIL_H_

#ifndef PARSE_EPSILON_H_
#define PARSE_EPSILON_H_
int parse_epsilon(char **pos);
#endif // PARSE_EPSILON_H_

#ifndef PARSE_MULT_OP_H_
#define PARSE_MULT_OP_H_
int parse_mult_op(char **pos);
#endif // PARSE_MULT_OP_H_

#ifndef PARSE_ADD_OP_H_
#define PARSE_ADD_OP_H_
int parse_add_op(char **pos);
#endif // PARSE_ADD_OP_H_

