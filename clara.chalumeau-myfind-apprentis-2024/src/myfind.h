#ifndef MYFIND_H
#define MYFIND_H
//#define _XOPEN_SOURCE 600
//#define _GNU_SOURCE

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <limits.h> /* PATH_MAX */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ast_evaluation.h"
#include "expression.h"
#include "parser.h"

int find(unsigned args_length, char **argv);
int myfind(unsigned args_length, char **argv);
int ls(char *start, char **argv, int length);
int myfind_d(unsigned args_length, char **argv);
int ls_d(char *start, char **argv, int length);
int myfind_h(unsigned args_length, char **argv);
int myfind_l(unsigned args_length, char **argv);
int ls_l(char *start, char **argv, int length);
int isDirectory(const char *path);
int isLink(const char *path);
void addFilename(char *filename, char *path, char *d_name);
int eval_expr_print(struct my_expr *ast_node, char *path);

#endif /* ! MEMORY_H */