#include <err.h>
#include <stdio.h>

#include "myfind.h"

int main(int argc, char **argv)
{
    // static struct my_expr *expr;

    if (argc < 1)
        errx(1, "Usage: %s <token> ([token] ...)", argv[0]);

    unsigned args_length = argc - 1;
    return find(args_length, argv + 1);
}
