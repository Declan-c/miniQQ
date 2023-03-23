#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERRLOG(errmsg)                                          \
    do                                                          \
    {                                                           \
        perror(errmsg);                                         \
        printf("%s - %s - %d\n", __FILE__, __func__, __LINE__); \
    } while (0)

#define LISTEN_MAX 10   //listen函数同时处理请求的最大数

//创建服务器链接，包括socket、bind、listen
int server_create(const char *ip, int port);

//封装accept函数，阻塞等待用户连接
int server_accept(int sockfd);

//发送消息
int send_msg(int cfd, const void *buf, int buf_len);


#endif