#include <stdio.h>
#include <stdlib.h>

#include "expression.h"

int eval_expr(struct my_expr *expr)
{
    if (expr->type == EXPR_ADDITION)
    {
        return eval_expr(expr->data.children.left)
            + eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_SUBTRACTION)
    {
        return eval_expr(expr->data.children.left)
            - eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_MULTIPLICATION)
    {
        return eval_expr(expr->data.children.left)
            * eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_DIVISION)
    {
        if (expr->data.children.right->data.value == 0)
        {
            fprintf(stderr, "Division by zero not allowed!\n");
            exit(1);
        }
        return eval_expr(expr->data.children.left)
            / eval_expr(expr->data.children.right);
    }
    else if (expr->type == EXPR_NEGATION)
    {
        return -expr->data.children.left->data.value;
    }
    else
    {
        return expr->data.value;
    }
}