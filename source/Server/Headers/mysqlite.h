#ifndef __MYSQLITE_H__
#define __MYSQLITE_H__
#include <sqlite3.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum getdata_flags      //mysqlite_data flags
{
    ONLINE,
    FIND    
};

//打开数据库
int mysqlite_open();

//数据库簇函数 -- 增 删 改
int mysqlite_exec(const char *sql);

//获取单个用户的数据
int mysqlite_getdata(const char * sql, void *data, int flags);

//获取多个用户数据
int mysqlite_get_table(const char *sql, char ***result, int *row, int *col);

//关闭数据库
int mysqlite_close();

#endif