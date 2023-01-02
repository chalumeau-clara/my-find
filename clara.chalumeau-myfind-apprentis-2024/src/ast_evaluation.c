#include "ast_evaluation.h"

int is_action(enum node_type type)
{
    // printf("is action %i\n", type);
    if (type >= 9)
        return true;
    return false;
}

int eval_expr(struct my_expr *ast_node, char *path, int *must_print)
{
    // printf("path : %s\n", path);
    // printf("ast node : %d\n", ast_node->type);
    if (ast_node->type == EXPR_AND)
    {
        return eval_expr(ast_node->data.children.left, path, must_print)
            && eval_expr(ast_node->data.children.right, path, must_print);
    }
    else if (ast_node->type == EXPR_OR)
    {
        return eval_expr(ast_node->data.children.left, path, must_print)
            || eval_expr(ast_node->data.children.right, path, must_print);
    }
    else if (ast_node->type == EXPR_NOT)
    {
        return !eval_expr(ast_node->data.children.left, path, must_print);
    }
    else
    {
        if (path == NULL && ast_node->type != EXPR_EXEC_PLUS)
            return true;
        if (is_action(ast_node->type) == true)
            *must_print = false;
        // printf("must print in ast %i\n", *must_print);
        for (int i = 0; i < funs_len; i++)
            if (funs[i].type == ast_node->type)
                return funs[i].fun(ast_node->data.value, path);
    }
    return true;
}
