#ifndef EXPRESSION_H
#define EXPRESSION_H

enum my_expr_type
{
    EXPR_ADDITION = 0,
    EXPR_SUBTRACTION,
    EXPR_MULTIPLICATION,
    EXPR_DIVISION,
    EXPR_NEGATION,
    EXPR_NUMBER
};

struct my_expr
{
    enum my_expr_type type;
    union
    {
        struct
        {
            struct my_expr *left;
            struct my_expr *right;
        } children;
        int value;
    } data;
};

#endif /* ! EXPRESSION_H */
