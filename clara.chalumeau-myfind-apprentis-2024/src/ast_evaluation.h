#ifndef AST_EVALUATION_H
#define AST_EVALUATION_H
#include <stdbool.h>

#include "expression.h"
#include "parser.h"

int eval_expr(struct my_expr *ast_node, char *path, int *must_print);

#endif /*! AST_EVALUATION_H */