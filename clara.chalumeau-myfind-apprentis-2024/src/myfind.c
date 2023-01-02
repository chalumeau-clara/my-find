#include "myfind.h"

int get_delete(unsigned args_length, char **argv)
{
    for (unsigned i = 0; i < args_length; i++)
        if (strcmp(argv[i], "-delete") == 0)
            return true;
    return false;
}

int is_option(char *argv)
{
    if (strcmp(argv, "-d") == 0 || strcmp(argv, "-H") == 0
        || strcmp(argv, "-L") == 0 || strcmp(argv, "-P") == 0)
        return true;
    return false;
}

int find(unsigned args_length, char **argv)
{
    // Pass useless option
    while (args_length > 1 && is_option(*argv) && is_option(*(argv + 1)))
    {
        argv++;
        args_length--;
    }
    if ((args_length > 0 && strcmp(argv[0], "-d") == 0))
        return myfind_d(args_length - 1, argv + 1);
    else if (get_delete(args_length, argv) == true)
        return myfind_d(args_length, argv);
    else if (args_length > 0 && strcmp(argv[0], "-H") == 0)
        return myfind_h(args_length - 1, argv + 1);
    else if (args_length > 0 && strcmp(argv[0], "-L") == 0)
        return myfind_l(args_length - 1, argv + 1);
    else if (args_length > 0 && strcmp(argv[0], "-P") == 0)
        return myfind(args_length - 1, argv + 1);

    return myfind(args_length, argv);
}

unsigned find_expr(char **argv, unsigned length)
{
    for (unsigned i = 0; i < length; i++)
        if (argv[i][0] == '-' || argv[i][0] == '!' || argv[i][0] == '(')
            return i;
    return length;
}

int myfind(unsigned args_length, char **argv)
{
    unsigned expr = find_expr(argv, args_length);
    // if no path is given
    if (expr == 0)
        ls(".", argv, args_length);
    else
    {
        unsigned i = 0;
        while (args_length != i && argv[i][0] != '-' && argv[i][0] != '!'
               && argv[i][0] != '(')
        {
            ls(argv[i], argv + expr, args_length - expr);
            i++;
        }
        if (args_length - expr != 0)
        {
            // for exec +
            static struct my_expr *expre;
            int must_print = true;
            expre = parse_expr(argv + expr, args_length - expr);
            eval_expr(expre, NULL, &must_print);
        }
    }
    return 0;
}

int ls(char *start, char **argv, int length)
{
    if (length == 0)
        printf("%s\n", start);
    else
    {
        static struct my_expr *expr;
        expr = parse_expr(argv, length);
        if (eval_expr_print(expr, start))
            printf("%s\n", start);
        free_expr(expr);
    }
    // printf("start %s, begin %s\n", start, begin);
    // Open directory
    DIR *dir;
    dir = opendir(start);

    // Read each under dir
    struct dirent *entry = readdir(dir);
    for (; entry; entry = readdir(dir))
    {
        // if (entry->d_name[0] != '.')
        if (strcmp(entry->d_name, ".") != 0
            && strcmp(entry->d_name,
                      "..")
                != 0) // to not infinite loop
        {
            char *dname = entry->d_name;
            char filename[PATH_MAX];
            addFilename(filename, start, dname);
            // printf("%s\n", filename);
            if (isDirectory(filename) == true)
                ls(filename, argv, length);
            else if (length == 0)
            {
                printf("%s\n", filename);
            }
            else
            {
                // printf("start : %s, filename %s \n",start, filename );
                static struct my_expr *expr;
                expr = parse_expr(argv, length);
                if (eval_expr_print(expr, filename))
                    printf("%s\n", filename);
                free_expr(expr);
            }
        }
    }

    closedir(dir);
    return 0;
}

int myfind_d(unsigned args_length, char **argv)
{
    unsigned expr = find_expr(argv, args_length);
    // if no path is given
    if (expr == 0)
        ls_d(".", argv, args_length);
    else
    {
        unsigned i = 0;
        while (args_length != i && argv[i][0] != '-' && argv[i][0] != '!'
               && argv[i][0] != '(')
        {
            ls_d(argv[i], argv + expr, args_length - expr);
            i++;
        }
        if (args_length - expr != 0)
        {
            // for exec +
            static struct my_expr *expre;
            int must_print = true;
            expre = parse_expr(argv + expr, args_length - expr);
            eval_expr(expre, NULL, &must_print);
        }
    }
    return 0;
}

int ls_d(char *start, char **argv, int length)
{
    // printf("start %s, begin %s\n", start, begin);
    // Open directory
    DIR *dir;
    dir = opendir(start);
    // Verify error

    // Read each under dir
    struct dirent *entry = readdir(dir);

    for (; entry; entry = readdir(dir))
    {
        // if (entry->d_name[0] != '.')
        if (strcmp(entry->d_name, ".") != 0
            && strcmp(entry->d_name,
                      "..")
                != 0) // to not infinite loop
        {
            char *dname = entry->d_name;
            char filename[PATH_MAX];
            addFilename(filename, start, dname);
            if (isDirectory(filename) == true)
                ls_d(filename, argv, length);
            else if (length == 0)
            {
                printf("%s\n", filename);
            }
            else
            {
                static struct my_expr *expr;
                expr = parse_expr(argv, length);
                if (eval_expr_print(expr, filename))
                    printf("%s\n", filename);
                free_expr(expr);
            }
        }
    }
    if (length == 0)
        printf("%s\n", start);
    else
    {
        static struct my_expr *expr;
        expr = parse_expr(argv, length);
        if (eval_expr_print(expr, start))
            printf("%s\n", start);
        free_expr(expr);
    }
    closedir(dir);
    return 0;
}

int myfind_h(unsigned args_length, char **argv)
{
    unsigned expr = find_expr(argv, args_length);
    // if no path is given
    if (expr == 0)
        ls(".", argv, args_length);
    else
    {
        unsigned i = 0;
        while (args_length != i && argv[i][0] != '-' && argv[i][0] != '!'
               && argv[i][0] != '(')
        {
            ls(argv[i], argv + expr, args_length - expr);
            i++;
        }
        if (args_length - expr != 0)
        {
            // for exec +
            static struct my_expr *expre;
            int must_print = true;
            expre = parse_expr(argv + expr, args_length - expr);
            eval_expr(expre, NULL, &must_print);
        }
    }
    return 0;
}

int myfind_l(unsigned args_length, char **argv)
{
    unsigned expr = find_expr(argv, args_length);
    // if no path is given
    if (expr == 0)
        ls_l(".", argv, args_length);
    else
    {
        unsigned i = 0;
        while (args_length != i && argv[i][0] != '-' && argv[i][0] != '!'
               && argv[i][0] != '(')
        {
            ls_l(argv[i], argv + expr, args_length - expr);
            i++;
        }
        if (args_length - expr != 0)
        {
            // for exec +
            static struct my_expr *expre;
            int must_print = true;
            expre = parse_expr(argv + expr, args_length - expr);
            eval_expr(expre, NULL, &must_print);
        }
    }
    return 0;
}

int ls_l(char *start, char **argv, int length)
{
    if (length == 0)
        printf("%s\n", start);
    else
    {
        static struct my_expr *expr;
        expr = parse_expr(argv, length);
        if (eval_expr_print(expr, start))
            printf("%s\n", start);
        free_expr(expr);
    }
    // printf("start %s, begin %s\n", start, begin);
    // Open directory
    DIR *dir;
    dir = opendir(start);
    // Verify error

    // Read each under dir
    struct dirent *entry = readdir(dir);

    for (; entry; entry = readdir(dir))
    {
        // if (entry->d_name[0] != '.')
        if (strcmp(entry->d_name, ".") != 0
            && strcmp(entry->d_name,
                      "..")
                != 0) // to not infinite loop
        {
            char *dname = entry->d_name;
            char filename[PATH_MAX];
            addFilename(filename, start, dname);
            if (isLink(filename) == true || isDirectory(filename) == true)
                ls_l(filename, argv, length);
            else if (length == 0)
            {
                printf("%s\n", filename);
            }
            else
            {
                static struct my_expr *expr;
                expr = parse_expr(argv, length);
                if (eval_expr_print(expr, filename))
                    printf("%s\n", filename);
                free_expr(expr);
            }
        }
    }

    closedir(dir);
    return 0;
}

int isDirectory(const char *path)
{
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int isLink(const char *path)
{
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1)
        return 0;
    return S_ISLNK(statbuf.st_mode);
}

void addFilename(char *filename, char *path, char *d_name)
{
    if (path[strlen(path) - 1] == '/')
        sprintf(filename, "%s%s", path, d_name);
    else
        sprintf(filename, "%s/%s", path, d_name);
}

int eval_expr_print(struct my_expr *ast_node, char *path)
{
    int must_print = true;
    if (eval_expr(ast_node, path, &must_print) == true && must_print == true)
        return true;
    // printf("must print %i\n", must_print);
    return false;
}
