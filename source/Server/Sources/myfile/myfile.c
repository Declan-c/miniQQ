#include "myfile.h"

FILE *myfopen(const char *file, const char *mode)
{
    FILE *fp = fopen(file, mode);
    if (NULL == fp)
    {
        perror("myfopen error");
        return NULL;
    }
    return fp;
}

int myfwrite(FILE *fp, const void *buf, int size)
{
    if (-1 == fwrite(buf, 1, size, fp))
    {
        perror("mywrite error");
        return -1;
    }
    return 0;
}

int myfread(FILE *fp, char *buf, int size)
{
    if (-1 == fread(buf, 1, size, fp))
    {
        perror("myread error");
        return -1;
    }
    return 0;
}

int myfclose(FILE *fp)
{
    if (-1 == fclose(fp))
    {
        perror("myclose error");
        return -1;
    }
    return 0;
}