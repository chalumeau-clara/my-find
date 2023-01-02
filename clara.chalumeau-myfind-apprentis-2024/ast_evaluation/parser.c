#include "parser.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

enum my_token_type
{
    TOKEN_PLUS = 0,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,
    TOKEN_NUMBER
};

struct my_token
{
    enum my_token_type type;
    int value;
};

struct my_tokens
{
    struct my_token *data;
    unsigned length;
};

struct token_model
{
    const char *str;
    enum my_token_type token_type;
};

static struct my_token parse_token(const char *str)
{
    static struct token_model exprs[] = { { "+", TOKEN_PLUS },
                                          { "-", TOKEN_MINUS },
                                          { "*", TOKEN_MULTIPLY },
                                          { "/", TOKEN_DIVIDE },
                                          { "(", TOKEN_LEFT_PARENTHESIS },
                                          { ")", TOKEN_RIGHT_PARENTHESIS } };

    struct my_token token = { TOKEN_NUMBER, 0 };
    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); ++i)
    {
 
        if (!strcmp(str, exprs[i].str))
        {
            token.type = exprs[i].token_type;
            return token;
        }
    }

    if (sscanf(str, "%d", &token.value) == 1)
        return token;

    errx(1, "Token not recognized: %s", str);
}

static const char *token_str(struct my_token *token)
{
    static const char *token_strs[] = { "+", "-", "*", "/", "(", ")" };
    static char number_str[11];

    if (token->type != TOKEN_NUMBER)
        return token_strs[token->type];

    sprintf(number_str, "%d", token->value);
    return number_str;
}

static struct my_expr *new_expr(void)
{
    return my_malloc(sizeof(struct my_expr));
}

static struct my_expr *clone_expr(struct my_expr *expr)
{
    return memcpy(new_expr(), expr, sizeof(*expr));
}

/**
 * Returns the index of the matching right parenthesis
 */
static unsigned find_parenthesis(struct my_token *tokens, unsigned index,
                                 unsigned length)
{
    unsigned level = 1;

    for (unsigned i = index; i < length; ++i)
    {
        enum my_token_type token_type = tokens[i].type;
        if (token_type == TOKEN_LEFT_PARENTHESIS)
            ++level;
        if (token_type == TOKEN_RIGHT_PARENTHESIS)
            --level;

        if (!level)
            return i;
    }

    errx(1, "Expected token: ')'");
}

static struct my_expr *parse_operations(struct my_token *tokens,
                                        unsigned *index, unsigned length);

static struct my_expr *parse_operand(struct my_token *tokens, unsigned *index,
                                     unsigned length)
{
    assert(*index < length);
    struct my_token *token = tokens + (*index)++;

    // Handle negation (e.g. "- 4")
    if (token->type == TOKEN_MINUS)
    {
        if (length == 1)
            errx(1, "Expected expression after token: -");

        struct my_expr expr;
        expr.type = EXPR_NEGATION;
        expr.data.children.left = parse_operand(tokens, index, length);
        return clone_expr(&expr);
    }
    if (token->type == TOKEN_NUMBER)
    {
        struct my_expr expr = { EXPR_NUMBER, { .value = token->value } };
        return clone_expr(&expr);
    }
    else if (token->type == TOKEN_LEFT_PARENTHESIS)
    {
        unsigned right = find_parenthesis(tokens, *index, length);
        struct my_expr *expr = parse_operations(tokens, index, right);
        *index = right + 1;
        return expr;
    }

    errx(1, "Expected tokens: -, '(' or number, but got: %s", token_str(token));
}

static struct my_expr *build_op_mult(struct my_expr **expr_stack,
                                     unsigned expr_stack_length,
                                     enum my_expr_type *op_stack,
                                     unsigned op_stack_length)
{
    if (!op_stack_length)
        return *expr_stack;

    struct my_expr expr;
    expr.type = op_stack[op_stack_length - 1];
    expr.data.children.left = build_op_mult(expr_stack, expr_stack_length - 1,
                                            op_stack, op_stack_length - 1);

    expr.data.children.right = expr_stack[expr_stack_length - 1];
    return clone_expr(&expr);
}

static struct my_expr *build_op_sum(struct my_expr **expr_stack,
                                    unsigned expr_stack_length,
                                    enum my_expr_type *op_stack,
                                    unsigned op_stack_length)
{
    /*
     * Here, i will overflow at the end of the loop.
     * No worries: overflow is perfectly defined for unsigned types.
     * In that case, 0 - 1 == ~0, which is not < op_stack_length.
     */
    for (unsigned i = op_stack_length - 1; i < op_stack_length; --i)
        if (op_stack[i] == EXPR_ADDITION || op_stack[i] == EXPR_SUBTRACTION)
        {
            struct my_expr expr;
            expr.type = op_stack[i];
            expr.data.children.left =
                build_op_sum(expr_stack, i + 1, op_stack, i);

            expr.data.children.right =
                build_op_mult(expr_stack + i + 1, expr_stack_length - (i + 1),
                              op_stack + i + 1, op_stack_length - (i + 1));
            return clone_expr(&expr);
        }

    return build_op_mult(expr_stack, expr_stack_length, op_stack,
                         op_stack_length);
}

static struct my_expr *parse_operations(struct my_token *tokens,
                                        unsigned *index, unsigned length)
{
    assert(*index < length);

    unsigned expr_stack_length = 0;
    struct my_expr **expr_stack = my_malloc(sizeof(*expr_stack));
    expr_stack[expr_stack_length++] = parse_operand(tokens, index, length);

    unsigned op_stack_length = 0;
    enum my_expr_type *op_stack = NULL;

    while (*index < length)
    {
        struct my_token *token = tokens + (*index)++;

        // If the token is not an operator
        if (!(token->type < TOKEN_LEFT_PARENTHESIS))
            errx(1, "Expected +, -, * or /, got %s", token_str(token));
        if (*index >= length)
            errx(1, "Expected expression after %s", token_str(token));

        expr_stack = my_reallocarray(expr_stack, expr_stack_length + 1,
                                     sizeof(*expr_stack));
        expr_stack[expr_stack_length++] = parse_operand(tokens, index, length);

        op_stack =
            my_reallocarray(op_stack, op_stack_length + 1, sizeof(*op_stack));
        // Legitimate cast: ops mapping between both enums matches
        int expr_type_int = token->type;
        op_stack[op_stack_length++] = expr_type_int;
    }

    struct my_expr *expr =
        build_op_sum(expr_stack, expr_stack_length, op_stack, op_stack_length);

    free(expr_stack);
    free(op_stack);

    return expr;
}

struct my_expr *parse_expr(char **strs, unsigned length)
{
    struct my_token *tokens = my_calloc(length, sizeof(struct my_token));

    for (unsigned i = 0; i < length; ++i)
        tokens[i] = parse_token(strs[i]);

    unsigned index = 0;
    struct my_expr *expr = parse_operations(tokens, &index, length);

    free(tokens);

    return expr;
}
