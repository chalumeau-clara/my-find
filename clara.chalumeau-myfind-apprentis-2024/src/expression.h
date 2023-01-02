#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <dirent.h>
#include <err.h>
#include <fnmatch.h>
#include <grp.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "memory.h"

#define funs_len 11

enum node_type
{
    // Operators
    EXPR_OR = 0,
    EXPR_AND,
    EXPR_NOT,

    // Operande
    // Test
    EXPR_NAME,
    EXPR_TYPE,
    EXPR_NEWER,
    EXPR_PERM,
    EXPR_USER,
    EXPR_GROUP,

    // Action
    EXPR_PRINT,
    EXPR_DELETE,
    EXPR_EXEC,
    EXPR_EXECDIR,
    EXPR_EXEC_PLUS,

};

struct my_expr
{
    enum node_type type;
    union
    {
        struct
        {
            struct my_expr *left;
            struct my_expr *right;
        } children;
        char *value;
    } data;
};

struct function
{
    enum node_type type;
    int (*fun)(char *, char *);
};

struct statfile
{
    char *string;
    __mode_t fun;
};
struct function funs[funs_len];
struct statfile type[7];

int print_fun(char *value, char *path);
int name_fun(char *value, char *path);
int type_fun(char *value, char *path);
int newer_fun(char *value, char *path);
int delete_fun(char *value, char *path);
int perm_fun(char *value, char *path);
int user_fun(char *value, char *path);
int group_fun(char *value, char *path);
int exec_fun(char *value, char *path);
int execdir_fun(char *value, char *path);
int exec_plus_fun(char *value, char *path);

#endif /* ! EXPRESSION_H */
