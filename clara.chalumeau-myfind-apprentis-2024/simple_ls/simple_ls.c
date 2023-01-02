#include <dirent.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "simple_ls: ./simple_ls [DIR] ");
        return 1;
    }

    errno = 0;
    DIR *dir;
    if (argc == 2)
        dir = opendir(argv[1]);
    else
        dir = opendir(".");

    if (dir == NULL)
    {
        switch (errno)
        {
        case EACCES:
            fprintf(stderr, "simple_ls: Permission denied\n");
            break;
        case ENOENT:
            fprintf(stderr, "simple_ls: Directory does not exist\n");
            break;
        case ENOTDIR:
            fprintf(stderr, "simple_ls: '%s' is not a directory\n", argv[1]);
            break;
        }
        return 1;
    }

    struct dirent *entry = readdir(dir);
    for (; entry; entry = readdir(dir))
    {
        if (entry->d_name[0] != '.')
            printf("%s\n", entry->d_name);
    }

    return 0;
}
