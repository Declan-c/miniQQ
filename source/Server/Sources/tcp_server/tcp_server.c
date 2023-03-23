#include "tcp_server.h"

//创建服务器链接，包括socket、bind、listen
int server_create(const char *ip, int port)
{
    int ret;
    //创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        ERRLOG("socket");
        return -1;
    }
    //设置地址可以重复绑定
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;   //用于存放服务器ip和端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    //绑定信息，将socket与固定的ip和端口绑定
    ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(-1 == ret)
    {
        ERRLOG("bind");
        return -1;
    }

    //设置监听
    ret = listen(sockfd, LISTEN_MAX);
    if(-1 == ret)
    {
        ERRLOG("listen");
        return -1;
    }

    return sockfd;
}

//accept函数，阻塞等待用户连接
int server_accept(int sockfd)
{
    struct sockaddr_in client_addr;   //用来保存请求连接的客户端信息
    int len = sizeof(client_addr);
    memset(&client_addr, 0, len);
    //阻塞等待客户端连接
    int cfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
    if(-1 == cfd)
    {
        ERRLOG("accept");
        return -1;
    }
    //s输出客户端信息
    printf("客户端(ip: %s | port: %d): cfd = %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), cfd);
    return cfd;
}

//发送消息
int send_msg(int cfd, const void *buf, int buf_len)
{
    int ret = send(cfd, buf, buf_len, 0);
    if(-1 == ret)
    {
        ERRLOG("send");
        return -1;
    }
    return 1;
}
