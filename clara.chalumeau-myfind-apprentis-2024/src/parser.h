#ifndef PARSER_H
#define PARSER_H

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_evaluation.h"
#include "expression.h"
#include "memory.h"
struct my_expr *parse_expr(char **strs, unsigned length);
void free_expr(struct my_expr *ast_node);
#endif /* ! PARSER_H */
