#include "parser.h"

enum my_token_type
{
    // Operators
    TOKEN_OR = 0,
    TOKEN_AND,
    TOKEN_NOT,
    TOKEN_LEFT_PARENTHESIS,
    TOKEN_RIGHT_PARENTHESIS,

    // Operande
    // Test
    TOKEN_NAME,
    TOKEN_TYPE,
    TOKEN_NEWER,
    TOKEN_PERM,
    TOKEN_USER,
    TOKEN_GROUP,

    // Action
    TOKEN_PRINT,
    TOKEN_DELETE,
    TOKEN_EXEC,
    TOKEN_EXECDIR,
    TOKEN_EXEC_PLUS
};

struct my_token
{
    enum my_token_type type;
    char value[4096];
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

// Return true if str is a token
bool is_token(char *str)
{
    static struct token_model exprs[] = { { "-a", TOKEN_AND },
                                          { "-o", TOKEN_OR },
                                          { "!", TOKEN_NOT },
                                          { "(", TOKEN_LEFT_PARENTHESIS },
                                          { ")", TOKEN_RIGHT_PARENTHESIS },
                                          { "-print", TOKEN_PRINT },
                                          { "-name", TOKEN_NAME },
                                          { "-type", TOKEN_TYPE },
                                          { "-newer", TOKEN_NEWER },
                                          { "-delete", TOKEN_DELETE },
                                          { "-perm", TOKEN_PERM },
                                          { "-user", TOKEN_USER },
                                          { "-group", TOKEN_GROUP },
                                          { "-exec", TOKEN_EXEC },
                                          { "-execdir", TOKEN_EXECDIR } };
    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); ++i)
    {
        if (strcmp(str, exprs[i].str) == 0)
        {
            return true;
        }
    }
    return false;
}

bool is_operator(enum my_token_type type)
{
    if (type >= 5)
        return false;
    return true;
}

// Parse the expression in brut with operator
static struct my_token parse_token(char **strs, unsigned *index,
                                   unsigned length)
{
    char *str = strs[*index];
    static struct token_model exprs[] = {
        { "-a", TOKEN_AND },
        { "-o", TOKEN_OR },
        { "!", TOKEN_NOT },
        { "(", TOKEN_LEFT_PARENTHESIS },
        { ")", TOKEN_RIGHT_PARENTHESIS },
        { "-print", TOKEN_PRINT },
        { "-name", TOKEN_NAME },
        { "-type", TOKEN_TYPE },
        { "-newer", TOKEN_NEWER },
        { "-delete", TOKEN_DELETE },
        { "-perm", TOKEN_PERM },
        { "-user", TOKEN_USER },
        { "-group", TOKEN_GROUP },
        { "-exec", TOKEN_EXEC }, // make a find plus function
        { "-execdir", TOKEN_EXECDIR }
    };

    struct my_token token = { TOKEN_AND, "\0" };
    for (unsigned i = 0; i < sizeof(exprs) / sizeof(*exprs); ++i)
    {
        if (!strcmp(str, exprs[i].str))
        {
            token.type = exprs[i].token_type;
            // printf("token %d\n", token.type);

            while (++(*index) < length && !is_token(strs[(*index)]))
            {
                if (strcmp(token.value, "\0") != 0)
                {
                    char tmp[4096] = "";
                    sscanf(strs[(*index)], "%s", tmp);
                    strcat(token.value, " ");
                    strcat(token.value, tmp);
                    if (token.type == TOKEN_EXEC
                        && strcmp(strs[(*index)], "+") == 0)
                        token.type = TOKEN_EXEC_PLUS;
                }
                else
                {
                    char tmp[4096] = "";
                    sscanf(strs[(*index)], "%s", tmp);
                    strcpy(token.value, tmp);
                }
            }
            return token;
        }
    }

    errx(1, "Token not recognized: %s", str);
}

static const char *token_str(struct my_token *token)
{
    static const char *token_strs[] = { "-o", "-a", "!", "-print", "(", ")" };
    static char number_str[4096];

    if (token->type < TOKEN_NAME)
        return token_strs[token->type];

    sprintf(number_str, "%s", token->value);
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

// Parse operands
static struct my_expr *parse_operand(struct my_token *tokens, unsigned *index,
                                     unsigned length)
{
    assert(*index < length);
    struct my_token *token = tokens + (*index)++;
    // printf("token %i\n", token->type);
    if (token->type >= TOKEN_NAME && token->type <= TOKEN_EXEC_PLUS)
    {
        enum node_type type = token->type - 2;
        // printf("type %d, %s\n", type, token->value);
        char *tmp_value = my_malloc(sizeof(token->value));
        strcpy(tmp_value, token->value);
        struct my_expr expr = { type, { .value = tmp_value } };
        // free(token->value);
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

static struct my_expr *build_op_and(struct my_expr **expr_stack,
                                    unsigned expr_stack_length,
                                    enum node_type *op_stack,
                                    unsigned op_stack_length)
{
    if (!op_stack_length)
        return *expr_stack;

    struct my_expr expr;
    expr.type = op_stack[op_stack_length - 1];
    expr.data.children.left = build_op_and(expr_stack, expr_stack_length - 1,
                                           op_stack, op_stack_length - 1);

    expr.data.children.right = expr_stack[expr_stack_length - 1];
    return clone_expr(&expr);
}

static struct my_expr *build_op_or(struct my_expr **expr_stack,
                                   unsigned expr_stack_length,
                                   enum node_type *op_stack,
                                   unsigned op_stack_length)
{
    /*
     * Here, i will overflow at the end of the loop.
     * No worries: overflow is perfectly defined for unsigned types.
     * In that case, 0 - 1 == ~0, which is not < op_stack_length.
     */
    for (unsigned i = op_stack_length - 1; i < op_stack_length; --i)
        if (op_stack[i] == EXPR_OR)
        {
            struct my_expr expr;
            expr.type = op_stack[i];
            expr.data.children.left =
                build_op_or(expr_stack, i + 1, op_stack, i);

            expr.data.children.right =
                build_op_and(expr_stack + i + 1, expr_stack_length - (i + 1),
                             op_stack + i + 1, op_stack_length - (i + 1));
            return clone_expr(&expr);
        }

    return build_op_and(expr_stack, expr_stack_length, op_stack,
                        op_stack_length);
}

static struct my_expr *parse_operations(struct my_token *tokens,
                                        unsigned *index, unsigned length)
{
    assert(*index < length);

    // Init operands stack (ex: -name)
    unsigned expr_stack_length = 0;
    struct my_expr **expr_stack = my_malloc(sizeof(*expr_stack));
    // Init operation stack (ex: AND)
    unsigned op_stack_length = 0;
    enum node_type *op_stack = NULL;

    // handle NOT
    if (tokens[*index].type == TOKEN_NOT)
    {
        struct my_token *token = tokens + (*index)++;
        op_stack =
            my_reallocarray(op_stack, op_stack_length + 1, sizeof(*op_stack));
        // Legitimate cast: ops mapping between both enums matches
        int expr_type_int = token->type;
        op_stack[op_stack_length++] = expr_type_int;
    }
    expr_stack[expr_stack_length++] = parse_operand(tokens, index, length);

    /*for (unsigned i = 0; i < expr_stack_length; i++)
        printf("expr stack before while: %d, %p\n", expr_stack[i]->type,
       expr_stack[i]->data.value);
    */

    while (*index < length)
    {
        struct my_token *token = tokens + (*index)++;
        // If the token is not an operator
        /*if (!(token->type < TOKEN_LEFT_PARENTHESIS)) {
            token->type = TOKEN_AND;
            --(*index);
        }*/
        if (*index >= length)
            break;

        expr_stack = my_reallocarray(expr_stack, expr_stack_length + 1,
                                     sizeof(*expr_stack));

        expr_stack[expr_stack_length++] = parse_operand(tokens, index, length);
        /*for (unsigned i = 0; i < expr_stack_length; i++)
                printf("expr stack i: %u: %d, %p\n",i, expr_stack[i]->type,
           expr_stack[i]->data.value);
          */

        op_stack =
            my_reallocarray(op_stack, op_stack_length + 1, sizeof(*op_stack));
        // Legitimate cast: ops mapping between both enums matches
        int expr_type_int = token->type;
        op_stack[op_stack_length++] = expr_type_int;
        /*for (unsigned i = 0; i < op_stack_length; i++)
            printf("op stack : %d\n", op_stack[i]);
            */
    }

    struct my_expr *expr =
        build_op_or(expr_stack, expr_stack_length, op_stack, op_stack_length);

    free(expr_stack);
    free(op_stack);

    return expr;
}

int countWord(const char *s, char *needle)
{
    // char result[PATH_MAX] = { '\0'};
    int i, cnt = 0;
    int needleLen = strlen(needle);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], needle) == &s[i])
        {
            cnt++;

            // Jumping to index after the old word.
            i += needleLen - 1;
        }
    }
    return cnt;
}

struct my_expr *parse_expr(char **strs, unsigned length)
{
    struct my_token *tokens = my_calloc(2 * length, sizeof(struct my_token));

    unsigned len = 0;
    // printf("len : %u, args %s\n", length, strs[0]);
    for (unsigned i = 0, j = 0; j < length; ++i)
    {
        struct my_token token = parse_token(strs, &j, length);
        // printf("op %i\n", is_operator(token.type));

        // Verify exec; and exec+
        if ((token.type == TOKEN_EXEC
             && token.value[strlen(token.value) - 1] != ';')
            || (token.type == TOKEN_EXEC_PLUS
                && token.value[strlen(token.value) - 1] != '+'))
            exit(EXIT_FAILURE);

        if (token.type == TOKEN_EXEC_PLUS
            && token.value[strlen(token.value) - 3] != '}'
            && token.value[strlen(token.value) - 4] != '{')
            errx(1, "myfind: missing argument to `-exec'");

        if (token.type == TOKEN_EXEC_PLUS && countWord(token.value, "{}") != 1)
            errx(1,
                 "myfind: only one instance of {} is supported with -exec ... "
                 "+");

        if (i != 0 && is_operator(token.type) == false
            && is_operator(tokens[i - 1].type) == false)
        {
            struct my_token token2 = { TOKEN_AND, "\0" };
            // printf("token add :%d, token after :%d\n", token2.type,
            // token.type);
            tokens[i++] = token2;
            // printf("token add :%d, token after :%d\n", tokens[i].type,
            //     token.type);
        }
        tokens[i] = token;
        len = i;
    }
    len++;

    /*for (unsigned i = 0; i < len; i++) {
        printf("tokens : %d\n", tokens[i].type);
        if (is_operator(tokens[i].type) == false)
            printf("value : %s\n", tokens[i].value);
    }
    printf("len : %u\n", len);
*/

    unsigned index = 0;
    struct my_expr *expr = parse_operations(tokens, &index, len);
    free(tokens);
    // printf("expr %d %s\n", expr->type, expr->data.value);
    // printf("\n");
    return expr;
}

void free_expr(struct my_expr *ast_node)
{
    if (ast_node->type == EXPR_AND)
    {
        free_expr(ast_node->data.children.left);
        free_expr(ast_node->data.children.right);
        free(ast_node);
    }
    else if (ast_node->type == EXPR_OR)
    {
        free_expr(ast_node->data.children.left);
        free_expr(ast_node->data.children.right);
        free(ast_node);
    }
    else if (ast_node->type == EXPR_NOT)
    {
        free_expr(ast_node->data.children.left);
        free(ast_node);
    }
    else
    {
        free(ast_node->data.value);
        free(ast_node);
    }
}
