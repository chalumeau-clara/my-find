#include "expression.h"

#include <stdbool.h>

struct function funs[funs_len] = {
    { .type = EXPR_PRINT, .fun = print_fun },
    { .type = EXPR_NAME, .fun = name_fun },
    { .type = EXPR_TYPE, .fun = type_fun },
    { .type = EXPR_NEWER, .fun = newer_fun },
    { .type = EXPR_DELETE, .fun = delete_fun },
    { .type = EXPR_PERM, .fun = perm_fun },
    { .type = EXPR_USER, .fun = user_fun },
    { .type = EXPR_GROUP, .fun = group_fun },
    { .type = EXPR_EXEC, .fun = exec_fun },
    { .type = EXPR_EXEC_PLUS, .fun = exec_plus_fun },
    { .type = EXPR_EXECDIR, .fun = execdir_fun }
};

struct statfile type[7] = {
    { .string = "b", .fun = __S_IFBLK }, { .string = "c", .fun = __S_IFCHR },
    { .string = "d", .fun = __S_IFDIR }, { .string = "f", .fun = __S_IFREG },
    { .string = "l", .fun = __S_IFLNK }, { .string = "p", .fun = __S_IFIFO },
    { .string = "s", .fun = __S_IFSOCK }
};

int print_fun(char *value, char *path)
{
    (void)value;
    //(void)path;
    printf("%s\n", path);
    return true;
}

int name_fun(char *value, char *path)
{
    // Get filename
    char *save_ptr = NULL;
    char tmp[4096];
    strcpy(tmp, path);

    if (strstr(path, "/"))
    {
        char *token = strtok_r(tmp, "/", &save_ptr);

        for (; token != NULL && strstr(save_ptr, "/") != NULL;
             token = strtok_r(NULL, "/", &save_ptr))
            ;
        // printf("save %s, token %s\n", save_ptr, token);
        if (path[strlen(path) - 1] == '/')
            save_ptr = token;
    }
    else
        save_ptr = path;
    // printf("value %s, save %s\n", value, save_ptr);
    if (fnmatch(value, save_ptr, 0) == 0)
        return true;
    return false;
}

int type_fun(char *value, char *path)
{
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1)
        warn("myfind: [type_fun] : %s cannot ope, stat", path);
    for (int i = 0; i < 7; i++)
        if (strcmp(type[i].string, value) == 0)
            return (statbuf.st_mode & __S_IFMT) == type[i].fun;
    err(1, "myfind: [type_fun] : %s is not a type", value);
    return false;
}

int newer_fun(char *value, char *path)
{
    struct stat statPath;
    if (stat(path, &statPath) == -1)
        warn("myfind: [newer_fun] : %s cannot open stat", path);

    struct stat statValue;
    if (stat(value, &statValue) == -1)
        warn("myfind: [newer_fun] : %s cannot open stat", value);

    if (statPath.st_mtim.tv_sec == statValue.st_mtim.tv_sec)
        return statPath.st_mtim.tv_nsec > statValue.st_mtim.tv_nsec;
    return statPath.st_mtim.tv_sec > statValue.st_mtim.tv_sec;
}

int delete_fun(char *value, char *path)
{
    (void)value;
    if (remove(path) != 0)
    {
        warn("myfind: [delete_fun] %s : No such file or directory ", path);
        return false;
    }
    return true;
}

int current_bits_perm(struct stat statValue, char *value)
{
    mode_t perm = statValue.st_mode;
    // Get perm remarque fprintf %o aurait aussi marche
    int u = (perm & S_IRUSR) ? 4 : 0;
    u += (perm & S_IWUSR) ? 2 : 0;
    u += (perm & S_IXUSR) ? 1 : 0;

    int g = (perm & S_IRGRP) ? 4 : 0;
    g += (perm & S_IWGRP) ? 2 : 0;
    g += (perm & S_IXGRP) ? 1 : 0;

    int o = (perm & S_IROTH) ? 4 : 0;
    o += (perm & S_IWOTH) ? 2 : 0;
    o += (perm & S_IXOTH) ? 1 : 0;
    // printf("u: %i g %i, o %i\n", u, g, o);
    if (u == value[0] - '0' && g == value[1] - '0' && o == value[2] - '0')
        return true;
    return false;
}

int have_perm(int perm, int value)
{
    if (value == 0)
        return true;
    // have exec
    if (value == 1 && (perm == 1 || perm == 3 || perm == 5 || perm == 7))
        return true;
    else if (value == 2 && (perm == 2 || perm == 3 || perm == 6 || perm == 7))
        return true;
    else if (value == 3 && (perm == 3 || perm == 7))
        return true;
    else if (value == 4 && perm >= 4)
        return true;
    else if (value == 5 && (perm == 5 || perm == 7))
        return true;
    else if (value == 6 && perm >= 6)
        return true;
    else if (value == 7 && perm == 7)
        return true;
    return false;
}

int all_bits_perm(struct stat statValue, char *value)
{
    mode_t perm = statValue.st_mode;
    // Get perm remarque fprintf %o aurait aussi marche
    int u = (perm & S_IRUSR) ? 4 : 0;
    u += (perm & S_IWUSR) ? 2 : 0;
    u += (perm & S_IXUSR) ? 1 : 0;

    int g = (perm & S_IRGRP) ? 4 : 0;
    g += (perm & S_IWGRP) ? 2 : 0;
    g += (perm & S_IXGRP) ? 1 : 0;

    int o = (perm & S_IROTH) ? 4 : 0;
    o += (perm & S_IWOTH) ? 2 : 0;
    o += (perm & S_IXOTH) ? 1 : 0;

    if (have_perm(u, value[1] - '0') == false)
        return false;
    if (have_perm(g, value[2] - '0') == false)
        return false;
    if (have_perm(o, value[3] - '0') == false)
        return false;
    return true;
}

int any_bits_perm(struct stat statValue, char *value)
{
    mode_t perm = statValue.st_mode;
    // Get perm remarque fprintf %o aurait aussi marche
    int u = (perm & S_IRUSR) ? 4 : 0;
    u += (perm & S_IWUSR) ? 2 : 0;
    u += (perm & S_IXUSR) ? 1 : 0;

    int g = (perm & S_IRGRP) ? 4 : 0;
    g += (perm & S_IWGRP) ? 2 : 0;
    g += (perm & S_IXGRP) ? 1 : 0;

    int o = (perm & S_IROTH) ? 4 : 0;
    o += (perm & S_IWOTH) ? 2 : 0;
    o += (perm & S_IXOTH) ? 1 : 0;

    if (have_perm(u, value[1] - '0') == true)
        return true;
    if (have_perm(g, value[2] - '0') == true)
        return true;
    if (have_perm(o, value[3] - '0') == true)
        return true;
    return false;
}

int perm_fun(char *value, char *path)
{
    struct stat statPath;
    if (lstat(path, &statPath) == -1)
        warn("myfind: [perm_fun] : %s cannot open stat", path);

    if (value[0] == '-')
        return all_bits_perm(statPath, value);
    else if (value[0] == '/')
        return any_bits_perm(statPath, value);
    return current_bits_perm(statPath, value);
}

char *getUser(uid_t uid)
{
    struct passwd *pws;
    pws = getpwuid(uid);
    if (pws == NULL)
        warn("myfind: [user_fun] : cannot get user");
    return pws->pw_name;
}

char *getGroup(uid_t uid)
{
    struct group *pws;
    pws = getgrgid(uid);
    if (pws == NULL)
        warn("myfind: [user_fun] : cannot get user");
    return pws->gr_name;
}

int user_fun(char *value, char *path)
{
    struct stat statPath;
    if (lstat(path, &statPath) == -1)
        warn("myfind: [user_fun] : %s cannot open stat", path);
    // printf("user :%s\n", getUser(statPath.st_uid));
    if (strcmp(getUser(statPath.st_uid), value) == 0)
        return true;
    return false;
}
int group_fun(char *value, char *path)
{
    struct stat statPath;
    if (lstat(path, &statPath) == -1)
        warn("myfind: [user_fun] : %s cannot open stat", path);
    if (strcmp(getGroup(statPath.st_gid), value) == 0)
        return true;
    return false;
}

char **build_exec_args(char *value, char *path)
{
    char **argv = my_malloc(sizeof(char *) * 4096);
    // argv[0] = strdup(path);
    char *save_ptr = value;
    char *token;
    int i = 0;
    // printf("value %s\n", value);
    while ((token = strtok_r(save_ptr, " ", &save_ptr)))
    {
        // printf("token: %s\n",token);
        if (strcmp(token, ";") == 0)
            break;
        if (strcmp(token, "{}") == 0)
            argv[i] = strdup(path);
        else
            argv[i] = strdup(token);
        // printf("argv %i = %s\n", i, argv[i]);
        i++;
    }
    argv[i] = NULL;
    /*printf("After parsing token\n");
    for (int j = 0; j < i; j++) {
        printf("tmp %i = %s\n", j, argv[j]);
    }*/
    (void)path;
    return argv;
}

void free_argv(char **argv)
{
    int i = 0;
    while (argv[i] != NULL)
    {
        free(argv[i]);
        i++;
    }
    free(argv);
    argv = NULL;
}

int exec_fun(char *value, char *path)
{
    char **argv = build_exec_args(value, path);
    // printf("value %s path %s\n", value, path);
    (void)path;
    pid_t pid = fork();

    if (pid == -1) // error
    {
        free_argv(argv);
        err(1, "myfind: error in pid -1");
    }
    if (pid == 0) // child
    {
        if (execvp(argv[0], argv) == -1)
        {
            free_argv(argv);
            err(0, "myfind: error in pid 0");
        }

        free_argv(argv);
        exit(true);
    }
    else // parent
    {
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            free_argv(argv);
            err(0, "myfind: error on waitPID\n");
        }
        free_argv(argv);
        if (WIFEXITED(status))
            return !WEXITSTATUS(status);
        return 0;
    }
}

int isDirectory2(const char *path)
{
    struct stat statbuf;
    if (lstat(path, &statbuf) == -1)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

void get_name(char *path, char filename[])
{
    char *base;
    base = basename(path);
    // char *file = strdup(path);
    // printf("base %s, dirname %s\n", base, basename(file));
    sprintf(filename, "./%s", base);
    // free(base);
    // return path;
}

int execdir_fun(char *value, char *path)
{
    // printf("path exec : %s\n", path);

    char filename[4096] = { '\0' };
    get_name(path, filename);
    char **argv = build_exec_args(value, filename);
    // printf("value %s path %s\n", value, path);
    (void)path;
    pid_t pid = fork();

    if (pid == -1) // error
    {
        free_argv(argv);
        err(1, "myfind: error in pid -1");
    }
    if (pid == 0) // child
    {
        char *real = NULL;
        real = realpath(path, real);
        real = dirname(real);
        chdir(real); // go in the directory
        free(real);
        if (execvp(argv[0], argv) == -1)
        {
            free(real);
            real = NULL;
            free_argv(argv);
            err(0, "myfind: error in pid 0");
        }
        free(real);
        free_argv(argv);
        exit(true);
    }
    else // parent
    {
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            free_argv(argv);
            err(0, "myfind: error on waitPID\n");
        }

        free_argv(argv);
        if (WIFEXITED(status))
            return !WEXITSTATUS(status);
        return 0;
    }
}

int exec_plus_fun_app(char **argv)
{
    // char **argv = build_exec_args(value, path);
    // printf("value %s path %s\n", value, path);
    //(void) path;
    pid_t pid = fork();

    if (pid == -1) // error
    {
        free_argv(argv);
        err(1, "myfind: error in pid -1");
    }
    if (pid == 0) // child
    {
        if (execvp(argv[0], argv) == -1)
        {
            free_argv(argv);
            err(0, "myfind: error in pid 0");
        }

        free_argv(argv);
        exit(true);
    }
    else // parent
    {
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            free_argv(argv);
            err(0, "myfind: error on waitPID\n");
        }
        free_argv(argv);
        if (WIFEXITED(status))
            return !WEXITSTATUS(status);
        return 0;
    }
}

int exec_plus_fun(char *value, char *path)
{
    static char **exec_file = NULL;
    static int nb_file = 0;

    if (path == NULL)
    {
        char **argv = my_malloc(sizeof(char *) * 4096);
        // argv[0] = strdup(path);
        char *save_ptr = value;
        char *token;
        int i = 0;
        // printf("value %s\n", value);
        while ((token = strtok_r(save_ptr, " ", &save_ptr)))
        {
            // printf("token: %s\n",token);
            // if (strcmp(token, "+") == 0)
            //  break;
            if (strcmp(token, "{}") == 0)
                break;
            else
                argv[i] = strdup(token);
            // printf("argv %i = %s\n", i, argv[i]);
            i++;
        }
        memcpy(argv + i, exec_file, sizeof(char *) * nb_file);
        argv[i + nb_file] = NULL;
        /*printf("After parsing token\n");
        for (int j = 0; j < i + nb_file; j++) {
            printf("tmp %i = %s\n", j, argv[j]);
        }*/
        return exec_plus_fun_app(argv);
    }
    else
    {
        exec_file = my_reallocarray(exec_file, nb_file + 1, sizeof(char *));
        exec_file[nb_file++] = strdup(path);
    }
    return 0;
}
