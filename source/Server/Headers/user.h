#ifndef __USER_H__
#define __USER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 用户结构体
typedef struct user
{
    char id[11];       // 客户端用户id
    char nickname[33]; // 用户昵称
    int cfd;           // 在线客户端的socket文件描述符
    int ban;           // 标记是否被禁言
    int vip;           // 标记是否为vip
    struct user *next;
} user_t;
// 在线用户链表
typedef struct user_linklist
{
    int count;    // 记录在线用户数量
    user_t *head; // 在线用户链表头结点
} ulist_t;

// 创建在线用户链表
int ulist_create();
// 头插法插入用户
int ulist_insert(int cfd, const char *user_id, const char *nickname);
// 通过用户id查找
user_t *ulist_search_byid(char *id);
// 通过客户端socket文件描述符查找
user_t *ulist_search_bycfd(int cfd);
// 通过cfd删除一个在线用户
int ulist_delete_bycfd(int cfd);
// 获得在线链表
ulist_t *get_ulist();
// 销毁在线用户链表
void ulist_destroy();

#endif