#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(stderr, "./simple_stat: [FILE] * n\n");
        return 1;
    }

    struct stat file_stat;

    for (int i = 1; i < argc; i++)
    {
        if (stat(argv[i], &file_stat) == -1)
        {
            fprintf(stderr, "./simple_stat: %d\n", errno);
            return 1;
        }
        else
        {
            fprintf(stdout, "st_dev=%lu\n", file_stat.st_dev);
            fprintf(stdout, "st_ino=%lu\n", file_stat.st_ino);
            fprintf(stdout, "st_mode=0%o\n", file_stat.st_mode);
            fprintf(stdout, "st_nlink=%lu\n", file_stat.st_nlink);
            fprintf(stdout, "st_uid=%u\n", file_stat.st_uid);
            fprintf(stdout, "st_gid=%u\n", file_stat.st_gid);
            fprintf(stdout, "st_rdev=%lu\n", file_stat.st_rdev);
            fprintf(stdout, "st_size=%lu\n", file_stat.st_size);
            fprintf(stdout, "st_atime=%lu\n", file_stat.st_atime);
            fprintf(stdout, "st_mtime=%lu\n", file_stat.st_mtime);
            fprintf(stdout, "st_ctime=%lu\n", file_stat.st_ctime);
            fprintf(stdout, "st_blksize=%lu\n", file_stat.st_blksize);
            fprintf(stdout, "st_blocks=%lu\n", file_stat.st_blocks);
        }
    }
    return 0;
}