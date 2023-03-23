#include "tcp_client.h"

static int sockfd;

// 连接服务器
int client_connect(const char *ip, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        ERRLOG("socket");
        return -1;
    }

    // 服务器信息
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(server_addr));
    if (-1 == ret)
    {
        ERRLOG("connect");
        close(sockfd);
        return -1;
    }
    return 0;
}

// 向服务器发送消息
int client_send(const void *buf, int buf_len)
{
    int ret = send(sockfd, buf, buf_len, 0);
    if (-1 == ret)
    {
        ERRLOG("send");
        return -1;
    }
    return 0;
}

// 从客户端接收消息
int client_recv(void *buf, int buf_len)
{
    int ret = recv(sockfd, buf, buf_len, 0);
    if (-1 == ret)
    {
        ERRLOG("recv");
        return -1;
    }
    return 0;
}

// 获取服务器的socket
int get_sockfd()
{
    return sockfd;
}

// 关闭socket
int client_close()
{
    if(-1 == close(sockfd))
    {
        ERRLOG("close");
        return -1;
    }
    return 0;
}