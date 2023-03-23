#ifndef __MYFILE_H__
#define __MYFILE_H__
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// 打开文件
FILE *myfopen(const char *file, const char *mode);
// 写文件
int myfwrite(FILE *fp, const void *buf, int size);
// 读文件
int myfread(FILE *fp, char *buf, int size);
// 关闭文件
int myfclose(FILE *fp);

#endif